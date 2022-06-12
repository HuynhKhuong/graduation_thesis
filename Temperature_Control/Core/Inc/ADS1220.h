#ifndef _ADS1220_H_
#define _ADS1220_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "main.h"
#include "stm32f1xx_hal_spi.h"
#include "stm32f1xx_hal_tim.h"
#include "math.h"
	
#define SPI_MASTER_DUMMY    			0xFF
#define RESET               			0x06   //Send the RESET command (06h) to make sure the ADS1220 is properly reset after power-up
#define START               			0x08    //Send the START/SYNC command (08h) to start converting in continuous conversion mode
#define WREG  										0x40
#define RREG  										0x20
#define CONFIG_REG0_ADDRESS 			0x00
#define CONFIG_REG1_ADDRESS 			0x01
#define CONFIG_REG2_ADDRESS 			0x02
#define CONFIG_REG3_ADDRESS 			0x03
#define REG_CONFIG0_MUX_MASK      0xF0
#define REG_CONFIG0_PGA_GAIN_MASK 0x0E
#define REG_CONFIG1_DR_MASK       0xE0
#define FSR 											(((long int)1<<23)-1)
#define VREF 											2.048//1.652855// // //2.048	// //            // Internal reference of 2.048V
#define VFSR 											VREF/1 
#define m_config_reg0 						0x60   //Default settings: AINP=AIN0, AINN=AIN1, Gain 1, PGA enabled
#define m_config_reg1 						0x04   //Default settings: DR=20 SPS, Mode=Normal, Conv mode=continuous, Temp Sensor disabled, Current Source off
#define m_config_reg2 						0x11   //Default settings: Vref REFP0-REFN0, 50/60Hz rejection, power open, IDAC 500uA
#define m_config_reg3 						0x80   //Default settings: IDAC1 = AIN3, IDAC2 Disabled , DRDY pin Only
#define a1 												0.00080069
#define b1												0.00019853 
#define c1												0.00000019194
#define a2 												0.00063
#define b2 												0.00022702
#define c2 												0.000000072949

#define CALIB
#undef CALIB
typedef struct 
{
	uint8_t Config_Reg0;
	uint8_t Config_Reg1;
	uint8_t Config_Reg2;
	uint8_t Config_Reg3;
	int32_t ADC_read;
	int32_t counter;
	float Temp;
	float R;
	float Vout;
}ADS1220_TypeDef;

void readVoltage(ADS1220_TypeDef *me, SPI_HandleTypeDef *my_hspi);
void WriteReg(uint8_t address, uint8_t value, SPI_HandleTypeDef *my_hspi);
uint8_t ReadReg(uint8_t address, SPI_HandleTypeDef *my_hspi);
void begin(ADS1220_TypeDef *me, SPI_HandleTypeDef *my_hspi);
void start_conv(SPI_HandleTypeDef *my_hspi);
int32_t Read_WaitForData(SPI_HandleTypeDef *my_hspi);
void Reset(SPI_HandleTypeDef *my_hspi);
float convert(ADS1220_TypeDef *me);
void Delay(uint32_t wait);
float calib(float R);
#ifdef __cplusplus
}
#endif
#endif
