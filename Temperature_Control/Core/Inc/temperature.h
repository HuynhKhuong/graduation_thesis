#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#ifdef __cplusplus
extern "C" {
#endif 
#include "main.h"
#include "stdio.h"
#include "string.h"



#define	BYTE_SIZE  								1
#define BLOCK_SIZE 								256
#define RECEIVE_DATA_SIZE 				264

typedef enum 
{
	USER_CMD 	=   0x00,
	GCODE 		= 	0x01,
} TransmissionID_TypeDef;

typedef enum
{
	G_CODE 		=		0x00,
	M_CODE		= 	0x01,
}Gcodecmd_Typedef;

typedef enum 
{
	SET_TEMP		 	= 	104,
	SET_P_Params	= 	130,
	SET_I_Params 	= 	131,
	SET_D_Params 	= 	132,
	NO_ACTION 		= 	0,
} GcodePacket_TypeDef;

typedef enum
{
	GCODE_OK 						= 0x00,
	GCODE_FAULT 				= 0x01,
	TRANSMISSION_FAULT 	= 0X02,
	NO_TRANSMISSION 		= 0x03,
} Status_typeDef;

//User's Class---------------------------------------------------------------------------------
typedef struct 
{
	TransmissionID_TypeDef TransmissionID;
	GcodePacket_TypeDef Gcode_packet;
	Gcodecmd_Typedef Gcode_mode;
	Status_typeDef	Status;
	float data_received;
	float P_Params;
	float I_Params;
	float D_Params;
	float P_Part;
	float I_Part;
	float D_Part;
	float Setpoint;
	float Temperature;
	float Error;
	float pre_Error;
	float pre_pre_Error;
	float T;
	int8_t Data_length;
	int8_t count;
	uint32_t Out;
	float	Control_Signal;
	float pre_Control_Signal;
	float Control_Signal_1;
}Temperature_Typedef;

	
Status_typeDef Protocoldecode(Temperature_Typedef *me, uint8_t *data, uint8_t size);
void protocolencode(Temperature_Typedef *me, char* data);
void Setup_params(Temperature_Typedef *me);
void Sepoint_set(Temperature_Typedef  *me);
void P_params_set(Temperature_Typedef *me);
void I_params_set(Temperature_Typedef *me);
void D_params_set(Temperature_Typedef *me);
void Temperature_Init(Temperature_Typedef *me);
int32_t PID_Control(Temperature_Typedef *me);


#ifdef __cplusplus
}
#endif
#endif
