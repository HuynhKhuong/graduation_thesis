#include "temperature.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
  * @brief  Excute changes made from cmd
  * @note   Changes are made after Protocoldecode
	* @param  Temperature_Typedef
  * @retval void
  */
void Setup_params(Temperature_Typedef *me)
{
	switch (me->Gcode_packet)
	{
		case 	SET_TEMP: 
		{
			Sepoint_set(me);
			break;
		}
		case SET_P_Params:
		{
			P_params_set(me);
			break;
		}
		case SET_I_Params:
		{
			I_params_set(me);
			break;
		}
		case SET_D_Params:
		{
			D_params_set(me);
			break;
		}
	}
}

/**
  * @brief  Decode the Protocol Frame between PC and STM32F407.
  * @note   Protocol includes: [0x28][0x7B][G/C][Cmd][Ddata][0x7D][0x29]
  * @param  received_data Protocol arrays
	* @param  received_data size
	* @param	Temperature_Typedef
  * @retval Status_typeDef
  */
 
Status_typeDef Protocoldecode(Temperature_Typedef *me, uint8_t *data, uint8_t size)

{
	char packet_data[2];
	char raw_data[4];

	for (uint8_t i = 0; i<2; i++) packet_data[i] = data[i+5];
	for (uint8_t i = 0; i<4; i++) raw_data[i] = data[i+7];
	
	if ((data[0] == 0x28) && (data[1] == 0x7B) && (data[size-1] == 0x29) && (data[size-2] == 0x7D)) //Check header file
	{
		me -> Data_length = data[3];
		if ((size - 4) == me -> Data_length)			//Check frame's size
		{
			if (data[4] == 1) 											//Get G-code
			{
				me -> TransmissionID = GCODE;
				if (data[5] == 'G') me -> Gcode_mode = G_CODE;
				else if (data[5] == 'M') me -> Gcode_mode = M_CODE;
				else 
				{
					me->Status = GCODE_FAULT;
					return GCODE_FAULT;
				}
			}
			else if (data[4] == 0) 
			{
				me -> TransmissionID = USER_CMD;
			}
			else 
			{
				me->Status = GCODE_FAULT;
				return GCODE_FAULT;
			}
		
			switch ((uint8_t)atof(packet_data))					//Check what the G-code wants to do
			{
				case 104: 
				{
					me -> Gcode_packet = SET_TEMP;
					break;
				}
				case 130:
				{
					me ->  Gcode_packet = SET_P_Params;
					break;
				}
				case 131:
				{
					me ->  Gcode_packet = SET_I_Params;
					break;
				}
				case 132:
				{
					me ->  Gcode_packet = SET_D_Params;
					break;
				}
				default: 
				{
					me->Status = GCODE_FAULT;
					return GCODE_FAULT;
				}	
			}
			
			me -> data_received = (float)atof(raw_data); //Get value to change
			me->Status = GCODE_OK;
			return GCODE_OK;
		}
		else 
		{
			me->Status = TRANSMISSION_FAULT;
			return TRANSMISSION_FAULT;
		}
	}
	else 																			//if error caught between transmission header, it should be transmission fault
	{
		me->Status = TRANSMISSION_FAULT;
		return TRANSMISSION_FAULT;	
	}						
}

/**
  * @brief  Encode result to feedback to PC 
  * @note   Protocol includes: [0x28][0x7B][G/C][Cmd][Ddata][0x7D][0x29]
  * @param  received_data Protocol arrays
	* @param  decoded data arrays
  * @retval Eeprom_Typedef
  */
void protocolencode(Temperature_Typedef *me, char* data)
{
	data[0] = 0x28;
	data[1] = 0x7B;
	sprintf((data+3),"%d",me->TransmissionID);
	sprintf((data+4),"%d",me->Gcode_mode);
	sprintf((data+5),"%d",me->Gcode_packet);
	switch (me->Status)
	{
		case GCODE_OK:
		{
			data[2] = 13;
			data[8] = '0';
			sprintf((data+9),"%.2f",me->Temperature);
			data[15] = 0x7D;
			data[16] = 0x29;
			me->Status = NO_TRANSMISSION;
			break;
		}
		case GCODE_FAULT:
		{
			data[2] = 13;
			data[8] = '1';
			sprintf((data+9),"%.2f",me->Temperature);
			data[15] = 0x7D;
			data[16] = 0x29;
			me->Status = NO_TRANSMISSION;
			break;
		}
		case TRANSMISSION_FAULT:
		{
			data[2] = 13;
			data[8] = '2';
			sprintf((data+9),"%.2f",me->Temperature);
			data[15] = 0x7D;
			data[16] = 0x29;
			me->Status = NO_TRANSMISSION;
			break;
		}
		case NO_TRANSMISSION:
		{
			data[5] = '0';
			data[6] = '0';
			data[7] = '0';
			data[2] = '1';
			sprintf((data+8),"%.2f",me->Temperature);
			data[14] = 0x7D;
			data[15] = 0x29;
			break;
		}
	}
}


void P_params_set(Temperature_Typedef *me)
{
	me->P_Params = me -> data_received;
}

void I_params_set(Temperature_Typedef *me)
{
	me->I_Params = me -> data_received;
}

void D_params_set(Temperature_Typedef *me)
{
	me->D_Params = me -> data_received;
}

void Sepoint_set(Temperature_Typedef *me)
{
	me->Setpoint = me -> data_received;
}
int32_t PID_Control(Temperature_Typedef *me)
{

		me->Error = me->Setpoint - me->Temperature;
		me->P_Part = me->P_Params*(me->Error - me->pre_Error);
		me->I_Part = 0.5*me->I_Params*me->T*(me->Error + me->pre_Error);
		me->D_Part = me->D_Params/me->T*( me->Error - 2*me->pre_Error+ me->pre_pre_Error);
		me->Control_Signal = me->pre_Control_Signal + me->P_Part + me->I_Part + me->D_Params;
		me->Control_Signal_1 = me->Control_Signal;
		if (me->Control_Signal < 0) me->Control_Signal = 0;
		else if (me->Control_Signal > 1599) me->Control_Signal = 1599;
		me->pre_pre_Error = me->pre_Error;
		me->pre_Error = me->Error;
		me->pre_Control_Signal = me->Control_Signal;
	return (int32_t)(me->Control_Signal);
}
void Temperature_Init(Temperature_Typedef *me)
{
	me->TransmissionID = GCODE;
	me->Gcode_packet = NO_ACTION;
	me->Gcode_mode = M_CODE;
	me->Status = NO_TRANSMISSION;
	me->data_received = 0;
	me->P_Params = 114;
	me->I_Params = 0;//0.0077;
	me->D_Params = 0;//0.1033;
	me->P_Part = 0;
	me->I_Part=0;
	me->D_Part=0;
	me->Setpoint=200;
	me->Temperature=0;
	me-> Error=0;
	me->pre_Error=0;
	me->pre_pre_Error=0;
	me->T=0.11;
	me->Data_length=0;
	me->Control_Signal=0;
	me->pre_Control_Signal=0;
}


