/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"
#include "colton.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ALL_LED_OFF 0
#define LEFT_LED_ON 1
#define RIGHT_LED_ON 2


#define JOY_LEFT 0
#define JOY_RIGHT 3
#define JOY_DOWN 2
#define JOY_CENTRE 4

#define GREEN 	0
#define ORANGE 	1
#define RED		2
#define BLUE	3

#define LED_OFF 0
#define LED_ON	1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void 	readJoysticks(int joyStickState[], int size);
void	debounce(float y[],int size);
void 	StateMachine();
void 	setAllLedOff();
void 	setLeftLedOn();
void	setRightLedOn();


void 	readJoysticks2(int joyStickState[], int size);
void	WriteLED(int LED_Colour, int LED_Status);
void 	StateMachine2();
void 	setAllLedOff2();
void 	setLeftLedOn2();
void	setRightLedOn2();
void 	debounce2(float y[],int size);
//void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
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
  MX_I2C1_Init();
  MX_USB_HOST_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
    /*COLTON'S CODE start*/
//    GPIO_PinState joy_c_status = HAL_GPIO_ReadPin(JOY_C_GPIO_Port, JOY_C_Pin);
//    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, joy_c_status);
//    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
//    WriteLED(BLUE, LED_ON);
//    HAL_Delay(100);
//    HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
//    HAL_Delay(100);
//    HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
//    HAL_Delay(100);
//    HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
//    HAL_Delay(100);
//    /*COLTON'S CODE end*/
//    /*State Machine Start*/
//    int joyStateStatus[5];
//	readJoysticks(joyStateStatus,5);
//    if(joyStateStatus[0] == 1){
//    	HAL_Delay(500);
//    }
    /*State machine Start*/
//    StateMachine();
    StateMachine2();
    HAL_Delay(100);
//    WriteLED(BLUE, LED_OFF);

  }
  /* USER CODE END 3 */
}

/*Colton Functions Start*/
void readJoysticks(int joyStickState[], int size)
{

	GPIO_PinState joy_a_status = HAL_GPIO_ReadPin(JOY_A_GPIO_Port, JOY_A_Pin);
	GPIO_PinState joy_b_status = HAL_GPIO_ReadPin(JOY_B_GPIO_Port, JOY_B_Pin);
	GPIO_PinState joy_c_status = HAL_GPIO_ReadPin(JOY_C_GPIO_Port, JOY_C_Pin);
	GPIO_PinState joy_d_status = HAL_GPIO_ReadPin(JOY_D_GPIO_Port, JOY_D_Pin);
	GPIO_PinState joy_ctr_status = HAL_GPIO_ReadPin(JOY_CTR_GPIO_Port, JOY_CTR_Pin);

	joyStickState[0] = joy_a_status;
	joyStickState[1] = joy_b_status;
	joyStickState[2] = joy_c_status;
	joyStickState[3] = joy_d_status;
	joyStickState[4] = joy_ctr_status;
}

void debounce(float y[],int size)
{
	int joyStateStatus[size];
	float a = 0.5;
	float b = 1 - a;

	//zero the incoming array
	for(int i = 0; i < size; i++){
		y[i] = 0;
	}

	//pseudocode mentioned in assignment
	for(int i = 0; i < 10; i++)
	{
		//read the current state of the joySticks
		readJoysticks(joyStateStatus,5);

		//Set joyStatePreviousStatus equal to what was last read
		for(int i = 0; i <= 4; i++){
			y[i] = a*y[i] + b*joyStateStatus[i];
		}
		HAL_Delay(1);
	}
}

void StateMachine()
{
    float joyStateStatus[5];
    debounce(joyStateStatus,5);
//	readJoysticks(joyStateStatus,5);
	static int currentState = ALL_LED_OFF;

	if(currentState == ALL_LED_OFF){
		if(joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
	}
	else if(currentState == LEFT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn();
		}
	}
	else if(currentState == RIGHT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff();
		}
		else if (joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn();
		}
	}


}
void 	setAllLedOff()
{
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 0); //left
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, 0); //right
}
void 	setLeftLedOn()
{
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 1); //left
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, 0); //right
}
void	setRightLedOn()
{
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, 0); //left
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, 1); //right
}

void readJoysticks2(int joyStickState[], int size)
{
	//Joy A and B are Port A
	//Joy C is Port B
	//Joy D and CTR are Port C

	int port_A = GPIOA->IDR;
	int port_B = GPIOB->IDR;
	int port_C = GPIOC->IDR;

	int joy_a_status;
	int joy_b_status;
	int joy_c_status;
	int joy_d_status;
	int joy_ctr_status;

	//PORT A
	if((port_A & GPIO_PIN_4) != 0){
		joy_a_status = 1;
	}
	else{
		joy_a_status = 0;
	}

	if((port_A & GPIO_PIN_6) != 0){
		joy_b_status = 1;
	}
	else{
		joy_b_status = 0;
	}

	//PORT B
	if((port_B & GPIO_PIN_15) != 0){
		joy_c_status = 1;
	}
	else{
		joy_c_status = 0;
	}

	//PORT C
	if((port_C & GPIO_PIN_6) != 0){
		joy_d_status = 1;
	}

	else{
		joy_d_status = 0;
	}

	if((port_C & GPIO_PIN_7) != 0){
		joy_ctr_status = 1;
	}
	else{
		joy_ctr_status = 0;
	}


	joyStickState[0] = joy_a_status;
	joyStickState[1] = joy_b_status;
	joyStickState[2] = joy_c_status;
	joyStickState[3] = joy_d_status;
	joyStickState[4] = joy_ctr_status;
}

void WriteLED(int LED_Colour, int LED_Status)
{
	int gpioPin;
	int ledNowStatus;

	switch (LED_Colour) {
		case GREEN:
			gpioPin = GPIO_PIN_12;
			break;
		case ORANGE:
			gpioPin = GPIO_PIN_13;
			break;
		case RED:
			gpioPin = GPIO_PIN_14;
			break;
		case BLUE:
			gpioPin = GPIO_PIN_15;
			break;
	}

	//Current Status
	if((GPIOD->ODR & gpioPin) == gpioPin){
		ledNowStatus = LED_ON;
	}
	else{
		ledNowStatus = LED_OFF;
	}

	if(ledNowStatus != LED_Status){
		GPIOD->ODR = GPIOD->ODR ^ gpioPin; //Toggle LED
	}
}
void StateMachine2()
{
    float joyStateStatus[5];
    debounce(joyStateStatus,5);
//	readJoysticks(joyStateStatus,5);
	static int currentState = ALL_LED_OFF;

	if(currentState == ALL_LED_OFF){
		if(joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn2();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
	}
	else if(currentState == LEFT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff2();
		}
		else if (joyStateStatus[JOY_RIGHT] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn2();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = RIGHT_LED_ON;
			setRightLedOn2();
		}
	}
	else if(currentState == RIGHT_LED_ON){
		if(joyStateStatus[JOY_CENTRE] == 0){
			currentState = ALL_LED_OFF;
			setAllLedOff2();
		}
		else if (joyStateStatus[JOY_LEFT] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
		else if (joyStateStatus[JOY_DOWN] == 0){
			currentState = LEFT_LED_ON;
			setLeftLedOn2();
		}
	}


}
void 	setAllLedOff2()
{
	WriteLED(GREEN, LED_OFF); //left
	WriteLED(RED, LED_OFF); //right
}
void 	setLeftLedOn2()
{
	WriteLED(GREEN, LED_ON); //left
	WriteLED(RED, LED_OFF); //right
}
void	setRightLedOn2()
{
	WriteLED(GREEN, LED_OFF); //left
	WriteLED(RED, LED_ON); //right
}
void debounce2(float y[],int size)
{
	int joyStateStatus[size];
	float a = 0.5;
	float b = 1 - a;

	//zero the incoming array
	for(int i = 0; i < size; i++){
		y[i] = 0;
	}

	//pseudocode mentioned in assignment
	for(int i = 0; i < 10; i++)
	{
		//read the current state of the joySticks
		readJoysticks2(joyStateStatus,5);

		//Set joyStatePreviousStatus equal to what was last read
		for(int i = 0; i <= 4; i++){
			y[i] = a*y[i] + b*joyStateStatus[i];
		}
		HAL_Delay(1);
	}
}
/*Colton Functions Stop*/

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : JOY_A_Pin JOY_B_Pin */
  GPIO_InitStruct.Pin = JOY_A_Pin|JOY_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : JOY_C_Pin */
  GPIO_InitStruct.Pin = JOY_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(JOY_C_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : JOY_D_Pin JOY_CTR_Pin */
  GPIO_InitStruct.Pin = JOY_D_Pin|JOY_CTR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
