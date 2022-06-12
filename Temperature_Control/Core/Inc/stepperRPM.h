#ifndef _STEPPERRPM_H_
#define _STEPPERRPM_H_

#ifdef __cplusplus
extern "C" {
#endif 
	
#include "main.h"

#define STEP_RES 						1.8f
#define FIN_ROUND 					360
#define MAX_SPEED						6
#define MIN_SPEED						0
#define MAX_FREQ						18
#define MIN_FREQ						2
#define RECEIVE_LENGTH			20
#define TRANSMITT_LENGTH_1 	12
#define BASE_FREQ 					100000
typedef enum 
{
	HARDWARE_ERROR = 0,
	COMM_ERROR 		 = 1,
	COMM_SUCCESS   = 2,
	SET_ERROR 		 = 3,
	GET_ERROR			 = 4,
	SET_SUCCESS		 = 5,
	GET_SUCCESS 	 = 6,
}Status_typedef;

typedef enum
{
	false,
	true,
}bool;

typedef enum
{
	SET_FREQ 			 = 0,
	STOP					 = 1,
	SET_P_Params	 = 2,
	SET_I_Params 	 = 3,
	SET_D_Params 	 = 4,
	SET_TEMP 			 = 5,
	IDLE 					 = 6,
	PID_TOGGLE 		 = 7,
	BED_HEAT 			 = 8,
	LOCK					 = 9,
}Nonumberstate_typedef;

typedef struct
{
	float freq;
	float set_rpm;
	float cur_rpm;
	int data_1_length;
	bool PID_flag;
	bool Bed_flag;
	uint8_t lock;
	uint8_t data_receive_1[RECEIVE_LENGTH];
	uint8_t value[2];																	//save and change value for temperature control
	uint8_t data_transmitt_1[TRANSMITT_LENGTH_1];
	Nonumberstate_typedef currentState;
	Status_typedef currentStatus;
}stepMotor_typedef;


Status_typedef create(stepMotor_typedef* me);
Status_typedef set_speed(stepMotor_typedef* me, float speed, TIM_HandleTypeDef *my_tim);
Status_typedef get_speed(stepMotor_typedef* me);
Status_typedef idle(stepMotor_typedef *me);
Status_typedef protocol_decode(stepMotor_typedef* me);
Status_typedef dutycycle_set(stepMotor_typedef* me, int stop, TIM_HandleTypeDef *my_tim);
Status_typedef freq_set(stepMotor_typedef* me, TIM_HandleTypeDef *my_tim);
Status_typedef stop(stepMotor_typedef *me, TIM_HandleTypeDef *my_tim);
void response(stepMotor_typedef *me, float temperature);
double Sepoint_set(stepMotor_typedef *me);
double P_params_set(stepMotor_typedef *me);
double I_params_set(stepMotor_typedef *me);
double D_params_set(stepMotor_typedef *me);

#ifdef __cplusplus
}
#endif
#endif
