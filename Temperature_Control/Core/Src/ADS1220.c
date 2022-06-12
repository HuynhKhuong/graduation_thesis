#include "ADS1220.h"
#include "main.h"
#include "stm32f1xx_hal_tim.h"


void WriteReg(uint8_t address, uint8_t value, SPI_HandleTypeDef *my_hspi)
{
		uint8_t data = WREG|(address<<2);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(5);
		HAL_SPI_Transmit(my_hspi, &(data), 1,100);
    HAL_SPI_Transmit(my_hspi, &(value), 1,100);
    HAL_Delay(5);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

uint8_t ReadReg(uint8_t address, SPI_HandleTypeDef *my_hspi)
{
    uint8_t data = RREG|(address<<2);
	  uint8_t data2 = SPI_MASTER_DUMMY ;
	  uint8_t data3 = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(5);

    HAL_SPI_Transmit(my_hspi, &(data), 1,100);
		HAL_SPI_TransmitReceive(my_hspi,  &(data2),  &(data3), 1, 100);
		//HAL_SPI_Transmit(&hspi1, &(data2), 1, 100);
		//HAL_Delay(100);
		//HAL_SPI_Receive_DMA(&hspi1, &(data3), 1);
    HAL_Delay(5);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    return data3;
}

void begin(ADS1220_TypeDef *me, SPI_HandleTypeDef *my_hspi)
{

    HAL_Delay(100);
    //ads1220_Reset();
    //HAL_Delay(100);
		
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	
//	WriteReg(CONFIG_REG0_ADDRESS , m_config_reg0);
		WriteReg(CONFIG_REG1_ADDRESS , m_config_reg1,my_hspi);	  
		WriteReg(CONFIG_REG2_ADDRESS , m_config_reg2,my_hspi);
    WriteReg(CONFIG_REG3_ADDRESS , m_config_reg3,my_hspi);
		WriteReg(CONFIG_REG0_ADDRESS , m_config_reg0,my_hspi);
    HAL_Delay(100);
		me->Config_Reg0 = ReadReg(CONFIG_REG0_ADDRESS,my_hspi);
		me->Config_Reg1 = ReadReg(CONFIG_REG1_ADDRESS,my_hspi);
	  me->Config_Reg2 = ReadReg(CONFIG_REG2_ADDRESS,my_hspi);
		me->Config_Reg3 = ReadReg(CONFIG_REG3_ADDRESS,my_hspi);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    HAL_Delay(100);

    //delay(100);
}
void start_conv(SPI_HandleTypeDef *my_hspi)
{
		uint8_t data = START;
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_SPI_Transmit(my_hspi, &(data), 1,100);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
int32_t Read_WaitForData(SPI_HandleTypeDef *my_hspi)
{
		uint8_t data = SPI_MASTER_DUMMY ;
    uint8_t SPI_Buff[3];
		uint8_t *address = SPI_Buff;
    int32_t mResult32=0;
    long int bit24;

    if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)== GPIO_PIN_RESET)             //Wait for DRDY to transition low
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);                         							//Take CS low
        HAL_Delay(100);
				//Delay(100);
        for (int i = 0; i < 3; i++)
        {					
					HAL_SPI_TransmitReceive(my_hspi,  &(data),  address, 1, 100);
					address++;
        }
        HAL_Delay(100);
				//Delay(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);                  //  Clear CS to high

        bit24 = SPI_Buff[0];
        bit24 = (bit24 << 8) | SPI_Buff[1];
        bit24 = (bit24 << 8) | SPI_Buff[2];                                 // Converting 3 bytes to a 24 bit int

        bit24 = ( bit24 << 8 );
        mResult32 = bit24>> 8;                      // Converting 24 bit two's complement to 32 bit two's complement
    }
    return mResult32;
}
void Reset(SPI_HandleTypeDef *my_hspi)
{
    uint8_t data = RESET;
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_SPI_Transmit(my_hspi, &(data), 1,100);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
float convert(ADS1220_TypeDef *me)
{
	
	if (me->R < 6710)  me->Temp = 1/(a1+b1*log(me->R)+c1*(log(me->R)*(log(me->R))*(log(me->R)))) - 273;	 
	else me->Temp = 1/(a2+b2*log(me->R)+c2*(log(me->R)*(log(me->R))*(log(me->R)))) - 273;	 
	return me->Temp;
}
void readVoltage(ADS1220_TypeDef *me, SPI_HandleTypeDef *my_hspi)
{
	me->ADC_read = Read_WaitForData(my_hspi);	
	me->Vout =  ((float)(me->ADC_read)*VREF*2 ) /1 /16777216;
	me->R = me->Vout/0.00001;
	#ifdef CALIB
	me->R = calib(me->R);
	#endif 
}

float calib(float R)
{
	//Calib R measured with ADC to approximate R read with VOM
	//These scaled params are measured and interpolate
	if( R > 20000) return (R -= (0.016*R + 3600));
	if( R > 3000) return (R-= (0.21*R -580));
	return (R -= (0.065*R - 2.4));
}
