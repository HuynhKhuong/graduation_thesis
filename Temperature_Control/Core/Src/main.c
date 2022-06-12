/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ADS1220.h"
#include "math.h"
#include "stepperRPM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define PWM_MAX 3999
#define pPID_Parameter_21
#ifdef pPID_Parameter_21
	double pKp = 90, pKd = 15, pKi = 0.17, T = 0.201; 
#endif
#define a1 												0.00080069
#define b1												0.00019853 
#define c1												0.00000019194
#define a2 												0.00063
#define b2 												0.00022702
#define c2 												0.000000072949
#define SPI_MASTER_DUMMY    			0xFF

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
//This instance includes: PID and Setpoint changes; Stepper motor speed changes; PC Communication
ADS1220_TypeDef my_ADS1220;
stepMotor_typedef my_Motor; 

//Temperature_Typedef my_Temperature;
//These params are used for PID algorithm
double Error_value = 0, pre_Error_value = 0 , pre_pre_Error_value = 0, pre_pre_pre_Error_value = 0, pre_pre_pre_pre_Error_value = 0;
float Set_Temperature = 200, pre_Set_Temperature  = 0, pre_pre_Set_Temperature  = 0, pre_pre_pre_Set_Temperature  = 0;
float Temperature = 0  , pre_Temperature  = 0, pre_pre_Temperature 	 = 0, pre_pre_pre_Temperature  = 0, pre_pre_pre_pre_Temperature  = 0, pre_pre_pre_pre_pre_Temperature  = 0, pre_pre_pre_pre_pre_pre_Temperature  = 0, pre_pre_pre_pre_pre_pre_pre_Temperature = 0;
double out = 0, pre_out = 0, pre_pre_out = 0, pre_pre_pre_out = 0, pre_pre_pre_pre_out = 0, pre_pre_pre_pre_pre_out = 0, pre_pre_pre_pre_pre_pre_out = 0, pre_pre_pre_pre_pre_pre_pre_out = 0; 
double out_temp = 0, out_real = 0;
uint16_t out_int;
double P_part = 0, I_part = 0, D_part = 0;
int count = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void PID_Temperature(void);
float TemperatureRead(float R);
int32_t ReadData(void);
void Delay(uint32_t wait);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	create(&my_Motor);
	//ADS1220 configurations
	my_ADS1220.Config_Reg0 = ReadReg(CONFIG_REG0_ADDRESS,&hspi1);
	my_ADS1220.Config_Reg1 = ReadReg(CONFIG_REG1_ADDRESS,&hspi1);
	my_ADS1220.Config_Reg2 = ReadReg(CONFIG_REG2_ADDRESS,&hspi1);
	my_ADS1220.Config_Reg3 = ReadReg(CONFIG_REG3_ADDRESS,&hspi1);
	Reset(&hspi1);
	HAL_Delay(1000);
	my_ADS1220.Config_Reg0 = ReadReg(CONFIG_REG0_ADDRESS,&hspi1);
	my_ADS1220.Config_Reg1 = ReadReg(CONFIG_REG1_ADDRESS,&hspi1);
	my_ADS1220.Config_Reg2 = ReadReg(CONFIG_REG2_ADDRESS,&hspi1);
	my_ADS1220.Config_Reg3 = ReadReg(CONFIG_REG3_ADDRESS,&hspi1);
	begin(&my_ADS1220,&hspi1);
	start_conv(&hspi1);
	
	//A4988 Configurations
	HAL_GPIO_WritePin(GPIOB, DIR_Pin,GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	
	
	//74HC245 Configurations
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, my_Motor.data_receive_1, RECEIVE_LENGTH); //Initialize UART Received IDLE with PC
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	HAL_TIM_Base_Start_IT(&htim1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 15999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 3999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 160;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 3000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DIR_Pin */
  GPIO_InitStruct.Pin = DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	readVoltage(&my_ADS1220, &hspi1);
  Temperature = convert(&my_ADS1220);  ;
	if(my_Motor.PID_flag) 
	{
		PID_Temperature();
		if(my_Motor.Bed_flag) {if(!my_Motor.lock)__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, out_int);}
		else __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,out_int);
	}
	switch (my_Motor.currentState)
	{
		case IDLE: 
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else
			{
				my_Motor.currentStatus = idle(&my_Motor);
				response(&my_Motor, Temperature);			
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case SET_FREQ:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, 0);
			else
			{
				//Convert value
				int temp = (int)my_Motor.value[0];
				my_Motor.set_rpm = (float)(temp << 8 |my_Motor.value[1])/100;
				my_Motor.currentStatus = set_speed(&my_Motor, my_Motor.set_rpm, &htim3);
				response(&my_Motor, Temperature);
				my_Motor.currentState = IDLE;
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case STOP:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else
			{
				my_Motor.currentStatus = stop(&my_Motor,&htim3);
				response(&my_Motor, Temperature);
				my_Motor.currentState = IDLE;
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case SET_TEMP:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else
			{
				Set_Temperature = Sepoint_set(&my_Motor);
				response(&my_Motor, Temperature);	
				my_Motor.currentState = IDLE;				
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case SET_P_Params:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else
			{
				pKp = P_params_set(&my_Motor);
				response(&my_Motor, Temperature);	
				my_Motor.currentState = IDLE;				
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case SET_I_Params:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else
			{
				pKi = I_params_set(&my_Motor);
				response(&my_Motor, Temperature);	
				my_Motor.currentState = IDLE;				
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case SET_D_Params:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else
			{
				pKd = D_params_set(&my_Motor);
				response(&my_Motor, Temperature);	
				my_Motor.currentState = IDLE;				
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case BED_HEAT:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else 
			{	
				my_Motor.Bed_flag = my_Motor.Bed_flag?false:true;
				//Reset all PID computational values
				out = 0;
				out_temp = 0;
				out_int = 0;  
				out = 0;
				P_part = 0;
				I_part = 0;
				D_part = 0;
				pre_pre_Error_value = 0;
				pre_Error_value = 0;
				Error_value = 0;
				pre_out = 0;
				my_Motor.currentStatus = SET_SUCCESS;
				response(&my_Motor, Temperature);	
			}
			my_Motor.currentState = IDLE;
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case LOCK:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else 
			{
				my_Motor.lock = my_Motor.lock?0:1;
				my_Motor.currentStatus = SET_SUCCESS;
				response(&my_Motor, Temperature);	
			}
			my_Motor.currentState = IDLE;
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
		case PID_TOGGLE:
		{
			if (my_Motor.currentStatus == COMM_ERROR) response(&my_Motor, Temperature);
			else
			{
				if(my_Motor.PID_flag) //Turn off PID Controlling
				{
					my_Motor.PID_flag = false;
					//Reset all PID computational values
					out = 0;
					out_temp = 0;
					out_int = 0;  
					out = 0;
					P_part = 0;
					I_part = 0;
					D_part = 0;
					pre_pre_Error_value = 0;
					pre_Error_value = 0;
					Error_value = 0;
					pre_out = 0;
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
					if(!my_Motor.lock) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
					my_Motor.currentState = IDLE;
				}
				else  //Turn on PID Controlling
				{
					my_Motor.PID_flag = true;
					my_Motor.currentState = IDLE;
				}
				my_Motor.currentStatus = SET_SUCCESS;
				response(&my_Motor, Temperature);	
			}
			HAL_UART_Transmit_DMA(&huart1, my_Motor.data_transmitt_1, TRANSMITT_LENGTH_1);
			break;
		}
	}
	//printf ("%f %f \n",Position,Set_Position);
// HAL_UART_Transmit_DMA(&huart1, transmitt, 2);
			//printf ("%f %f \n",motor_speed1,Set_Frequency);
	
//		my_Temperature.Temperature = convert(&my_ADS1220);
//		convert(&my_ADS1220);//
//		my_Temperature.Out = PID_Control(&my_Temperature);
//		if(my_ADS1220.Temp > 0)
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,800);
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,my_Temperature.Out);
//		sprintf(transmitt,"%.2f",my_ADS1220.Temp);//my_Temperature.Temperature);//
//		transmitt[5] = '\n';
//		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)transmitt, 6);//transmitarray[2]+4);		
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
		my_Motor.data_1_length = Size;
		my_Motor.currentStatus = protocol_decode(&my_Motor);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, my_Motor.data_receive_1, RECEIVE_LENGTH); //Renitialize UART Received IDLE
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		HAL_UART_DeInit(&huart1);  
		HAL_UART_Init(&huart1);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, my_Motor.data_receive_1, RECEIVE_LENGTH); //Renitialize UART Received IDLE
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
}  
void PID_Temperature(void)
{
	
	if(count > 10) 
	{
		/*PID Calculation*/
		Error_value = Set_Temperature - Temperature; //gia tri xac din ref - gia tri encoder
//		P_part = (Error_value - pre_Error_value) * pKp; //cac he so trong PID //Kp*e
//		I_part = (Error_value + pre_Error_value) * 0.5 * pKi; //Ki*e/2
//		D_part = pKd/T* (Error_value - (2*pre_Error_value) + pre_pre_Error_value); //Kd *(e -2*e(k-1) + e(k-2))
//		out+=  P_part + I_part + D_part;
		P_part = pKp * Error_value;
		I_part += pKi * Error_value*T;
		D_part = pKd * (Error_value - pre_Error_value)/T;
		out =  P_part + I_part + D_part;
		if (out<0) 	
		{
			out = 0;
			out_temp = out;
		  out_int = (unsigned)(int)(rint(out)); //lam tron u va chuyen kieu thanh so nguyen 
		  out = out_temp;	
		  pre_pre_Error_value = pre_Error_value;
		  pre_Error_value = Error_value;
		  pre_out = out;
		}
		else if (out > PWM_MAX)
		{		
			 out = PWM_MAX;
			 out_temp = out;	
			 out_int = (unsigned)(int)(rint(out)); //lam tron u va chuyen kieu thanh so nguyen 
			 out = out_temp;	
			 pre_pre_Error_value = pre_Error_value;
			 pre_Error_value = Error_value;
			 pre_out = out;
		}
		
		// o day ta luu cac bien
		else
		{
			out_int = (unsigned)(int)(rint(out)); //lam tron u va chuyen kieu thanh so nguyen 
			//out = out_temp;	
			pre_pre_Error_value = pre_Error_value;
			pre_Error_value = Error_value;
			pre_out = out;
		}		
	}
	else count++;	
}
//int32_t ReadData(void)
//{
//	uint8_t *address = SPI_Buff;
//    if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)== GPIO_PIN_RESET)             //Wait for DRDY to transition low
//    {
//        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);                         							//Take CS low
//        HAL_Delay(50);
//        for (int i = 0; i < 3; i++)
//        {					
//					HAL_SPI_TransmitReceive(&hspi1,  &(data),  address, 1, 100);
//					address++;
//        }
//        HAL_Delay(50);
//        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);                  //  Clear CS to high

//        bit24 = SPI_Buff[0];
//        bit24 = (bit24 << 8) | SPI_Buff[1];
//        bit24 = (bit24 << 8) | SPI_Buff[2];                                 // Converting 3 bytes to a 24 bit int

//        bit24= ( bit24 << 8 );
//        mResult32 = ( bit24 >> 8 );                      // Converting 24 bit two's complement to 32 bit two's complement
//    }
//    return mResult32;
//}
//float TemperatureRead(float R)
//{
//	if (R < 6710)  return (1/(a1+b1*log(R)+c1*(log(R)*(log(R))*(log(R)))) - 273);	 
//	else return (1/(a2+b2*log(R)+c2*(log(R)*(log(R))*(log(R)))) - 273);	 
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
