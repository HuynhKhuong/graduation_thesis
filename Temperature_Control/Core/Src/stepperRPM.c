#include "main.h"
#include "stepperRPM.h"
#include "stm32f1xx_hal_tim.h"
#include "stdlib.h"

Status_typedef create(stepMotor_typedef* me)
{
	me->freq = 0;
	me->set_rpm = BASE_FREQ/TIM3->ARR * 60*STEP_RES/FIN_ROUND;
	me->cur_rpm = 0;
	me->currentState = IDLE;
	me->data_1_length = RECEIVE_LENGTH;
	me->PID_flag = false;
	me->Bed_flag = true;
	me->lock = 0;
	me->currentState = IDLE;
	return SET_SUCCESS;
}

Status_typedef stop(stepMotor_typedef *me, TIM_HandleTypeDef *my_tim)
{
	return dutycycle_set(me, 1,my_tim);
}

Status_typedef idle(stepMotor_typedef *me)
{
	//IDLE Mode: get temperature and get speed
	return(get_speed(me));

}

Status_typedef get_speed(stepMotor_typedef* me)
{
	me->cur_rpm = (float)(BASE_FREQ)/TIM3->ARR * (float)(60*STEP_RES)/FIN_ROUND;
	if(abs((int)(me->cur_rpm - me->set_rpm)) > 50) return GET_ERROR;  // If cur_rpm is distinct with (< 80%) set_rpm, report 
	else return GET_SUCCESS;
}

Status_typedef protocol_decode(stepMotor_typedef* me)
{
	//check headers
	if((me->data_receive_1[0] != '(' )|| (me->data_receive_1[1] != '{' )||(me->data_receive_1[me->data_1_length - 1] != ')' )||(me->data_receive_1[me->data_1_length - 2] != '}'))
		return COMM_ERROR;
	//check data length
	else if (me->data_receive_1[2] != me->data_1_length - 5) 		return COMM_ERROR;	
	//check command and take value
	else 
	{
		uint8_t flag = 0;
		uint8_t count = 0;
		switch(me->data_receive_1[3])
		{
			case 'S':											//actually with speed control, there's only a command
			{
				me->currentState = SET_FREQ;
				break;
			}
			case 'T':
			{
				me->currentState = SET_TEMP;
				break;
			}
			case 'P':
			{
				me->currentState = SET_P_Params;
				break;
			}
			case 'I':
			{
				me->currentState = SET_I_Params;
				break;
			}
			case 'D':
			{
				me->currentState = SET_D_Params;
				break;
			}
			case 'R':
			{
				me->currentState = PID_TOGGLE;
				break;
			}
			case 'B':
			{
				me->currentState = BED_HEAT;
				break;
			}
			case 'L':
			{
				me->currentState = LOCK;
			}
		}
		
		for (int i = 3; i < me->data_1_length ; i++)
			{
				switch (flag)
				{
					case 0: 
					{
						if (me->data_receive_1[i] == ':') flag = 1;
						break;
					}
					case 1:
					{
						if (me->data_receive_1[i] == '}') flag = 2;
						else
						{
							me->value[count] = me->data_receive_1[i];
							count++;
						}
						break;
					}
				}
			}
		return COMM_SUCCESS;
 }
}
Status_typedef dutycycle_set(stepMotor_typedef* me, int stop, TIM_HandleTypeDef *my_tim)
{
	switch(stop)
		{
			case 0: 
			{
				if ((int)(BASE_FREQ/me->freq/2) > TIM3->ARR) return SET_ERROR;
				else
				{
					__HAL_TIM_SET_COMPARE(my_tim, TIM_CHANNEL_1, (int)(BASE_FREQ/me->freq/2));
					return SET_SUCCESS;
				}
			}
			case 1:
			{
				__HAL_TIM_SET_COMPARE(my_tim, TIM_CHANNEL_1, 0);
				return SET_SUCCESS;		
			}
		}
}

Status_typedef freq_set(stepMotor_typedef* me, TIM_HandleTypeDef *my_tim)
{
	if ((me->freq > MAX_FREQ) || (me->freq < MIN_FREQ)) return SET_ERROR;
	else
	{
		//stop motor first
		__HAL_TIM_SET_COMPARE(my_tim,TIM_CHANNEL_1,0);

		//change timer T base on given freq
		TIM3->ARR = (int)(BASE_FREQ/me->freq);

		return SET_SUCCESS;
	}
}

Status_typedef set_speed(stepMotor_typedef* me, float speed, TIM_HandleTypeDef *my_tim)
{
	if (speed == 0) 
	{
		dutycycle_set(me, 1, my_tim);
		return SET_SUCCESS;
	}
	if (speed > MAX_SPEED) return SET_ERROR;
	else if (speed < MIN_SPEED) return SET_ERROR;
	else 
	{
		me->set_rpm = speed;
		me->freq = (float)me->set_rpm/((STEP_RES/FIN_ROUND)*60);
		if ((freq_set(me, my_tim) == SET_ERROR)) return SET_ERROR;
		if ((dutycycle_set(me, 0,my_tim) == SET_ERROR)) return SET_ERROR;
		return SET_SUCCESS;
	}
}
void response(stepMotor_typedef *me, float temperature)
{
	
	me->data_transmitt_1[0] = 0x28;
	me->data_transmitt_1[1] = 0x7B;
	me->data_transmitt_1[2] = me->currentStatus;
	switch (me->currentStatus)
	{
		case GET_ERROR: //Error: frequency set for timer doesn't fit the constraint								
		{								
			me->data_transmitt_1[3] = '-'; 
			me->data_transmitt_1[4] = 'T';
			me->data_transmitt_1[5] = 'x';//((int)(temperature * 100) >> 8)&0xFF;
			me->data_transmitt_1[6] = 'x';//(int)(temperature * 100)&0xFF;
			me->data_transmitt_1[7] = 'S';
			me->data_transmitt_1[8] = 'x';
			me->data_transmitt_1[9] = 'x';
			me->data_transmitt_1[10] = 0x7D;
			me->data_transmitt_1[11] = 0x29;
			break;
		}
		case HARDWARE_ERROR:
		{
			me->data_transmitt_1[3] = '-'; 
			me->data_transmitt_1[4] = 'T';
			me->data_transmitt_1[5] = 'x';//((int)(temperature * 100) >> 8)&0xFF;
			me->data_transmitt_1[6] = 'x';//(int)(temperature * 100)&0xFF;
			me->data_transmitt_1[7] = 'S';
			me->data_transmitt_1[8] = 'x';
			me->data_transmitt_1[9] = 'x';
			me->data_transmitt_1[10] = 0x7D;
			me->data_transmitt_1[11] = 0x29;
			break;
		}
		case COMM_ERROR: //Error: Communication standard mismatch
		{
			me->data_transmitt_1[3] = '-'; 
			me->data_transmitt_1[4] = 'T';
			me->data_transmitt_1[5] = 'x';
			me->data_transmitt_1[6] = 'x';
			me->data_transmitt_1[7] = 'S';
			me->data_transmitt_1[8] = 'x';
			me->data_transmitt_1[9] = 'x';
			me->data_transmitt_1[10] = 0x7D;
			me->data_transmitt_1[11] = 0x29;
		}
		default:
		{
			if(temperature <0)
			{
				me->data_transmitt_1[5] = 'x';
				me->data_transmitt_1[6] = 'x';
			}
			else if(temperature > 300) 
			{
				me->data_transmitt_1[5] = 'x';
				me->data_transmitt_1[6] = 'x';
			}
			else 
			{
				me->data_transmitt_1[5] = ((int)(temperature * 100) >> 8)&0xFF;
				me->data_transmitt_1[6] = (int)(temperature * 100)&0xFF;
			}
			me->data_transmitt_1[3] = '-'; 
			me->data_transmitt_1[4] = 'T';
			me->data_transmitt_1[7] = 'S';
			me->data_transmitt_1[8] = ((int)(me->cur_rpm*100)>> 8)&0xFF;
			me->data_transmitt_1[9] = (int)(me->cur_rpm*100) & 0xFF;
			me->data_transmitt_1[10] = 0x7D;
			me->data_transmitt_1[11] = 0x29;
			break;
		}
	}
}

double Sepoint_set(stepMotor_typedef *me)
{
	int temp;
	temp = me->value[0];
	temp = (temp << 8) | me->value[1];
	me->currentStatus = SET_SUCCESS;
	return (double)temp/100;
}
double P_params_set(stepMotor_typedef *me)
{
	int temp;
	temp = me->value[0];
	temp = (temp << 8) | me->value[1];
	me->currentStatus = SET_SUCCESS;
	return (double)temp/100;
}
double I_params_set(stepMotor_typedef *me)
{
	int temp;
	temp = me->value[0];
	temp = (temp << 8) | me->value[1];
	me->currentStatus = SET_SUCCESS;
	return (double)temp/100;
}
double D_params_set(stepMotor_typedef *me)
{
	int temp;
	temp = me->value[0];
	temp = (temp << 8) | me->value[1];
	me->currentStatus = SET_SUCCESS;
	return (double)temp/100;
}
