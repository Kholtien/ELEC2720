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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Serial_Driver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* Definitions for RxTask */
osThreadId_t RxTaskHandle;
const osThreadAttr_t RxTask_attributes = {
  .name = "RxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TxTask */
osThreadId_t TxTaskHandle;
const osThreadAttr_t TxTask_attributes = {
  .name = "TxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for RxTask2 */
osThreadId_t RxTask2Handle;
const osThreadAttr_t RxTask2_attributes = {
  .name = "RxTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TxTask2 */
osThreadId_t TxTask2Handle;
const osThreadAttr_t TxTask2_attributes = {
  .name = "TxTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for dummyTask1 */
osThreadId_t dummyTask1Handle;
const osThreadAttr_t dummyTask1_attributes = {
  .name = "dummyTask1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for dummyTask2 */
osThreadId_t dummyTask2Handle;
const osThreadAttr_t dummyTask2_attributes = {
  .name = "dummyTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for RxTaskQ */
osMessageQueueId_t RxTaskQHandle;
const osMessageQueueAttr_t RxTaskQ_attributes = {
  .name = "RxTaskQ"
};
/* Definitions for TxTaskQ */
osMessageQueueId_t TxTaskQHandle;
const osMessageQueueAttr_t TxTaskQ_attributes = {
  .name = "TxTaskQ"
};
/* Definitions for RxTask2Q */
osMessageQueueId_t RxTask2QHandle;
const osMessageQueueAttr_t RxTask2Q_attributes = {
  .name = "RxTask2Q"
};
/* Definitions for TxTask2Q */
osMessageQueueId_t TxTask2QHandle;
const osMessageQueueAttr_t TxTask2Q_attributes = {
  .name = "TxTask2Q"
};
/* Definitions for dummyTask1Q */
osMessageQueueId_t dummyTask1QHandle;
const osMessageQueueAttr_t dummyTask1Q_attributes = {
  .name = "dummyTask1Q"
};
/* Definitions for dummyTask2Q */
osMessageQueueId_t dummyTask2QHandle;
const osMessageQueueAttr_t dummyTask2Q_attributes = {
  .name = "dummyTask2Q"
};
/* Definitions for TxBufSema */
osSemaphoreId_t TxBufSemaHandle;
const osSemaphoreAttr_t TxBufSema_attributes = {
  .name = "TxBufSema"
};
/* Definitions for RxBufSema */
osSemaphoreId_t RxBufSemaHandle;
const osSemaphoreAttr_t RxBufSema_attributes = {
  .name = "RxBufSema"
};
/* Definitions for Tx2BufSema */
osSemaphoreId_t Tx2BufSemaHandle;
const osSemaphoreAttr_t Tx2BufSema_attributes = {
  .name = "Tx2BufSema"
};
/* Definitions for Rx2BufSema */
osSemaphoreId_t Rx2BufSemaHandle;
const osSemaphoreAttr_t Rx2BufSema_attributes = {
  .name = "Rx2BufSema"
};
/* USER CODE BEGIN PV */
stSERIAL_CHANNELTypeDef *pstSerialChannel2;
stSERIAL_CHANNELTypeDef *pstSerialChannel3;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
void fvdRxTask(void *argument);
void fvdTxTask(void *argument);
void fvdRxTask2(void *argument);
void fvdTxTask2(void *argument);
void fvddummyTask1(void *argument);
void fvddummyTask2(void *argument);

/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of TxBufSema */
  TxBufSemaHandle = osSemaphoreNew(2, 2, &TxBufSema_attributes);

  /* creation of RxBufSema */
  RxBufSemaHandle = osSemaphoreNew(2, 2, &RxBufSema_attributes);

  /* creation of Tx2BufSema */
  Tx2BufSemaHandle = osSemaphoreNew(2, 2, &Tx2BufSema_attributes);

  /* creation of Rx2BufSema */
  Rx2BufSemaHandle = osSemaphoreNew(2, 2, &Rx2BufSema_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of RxTaskQ */
  RxTaskQHandle = osMessageQueueNew (16, sizeof(uint8_t), &RxTaskQ_attributes);

  /* creation of TxTaskQ */
  TxTaskQHandle = osMessageQueueNew (16, sizeof(uint8_t), &TxTaskQ_attributes);

  /* creation of RxTask2Q */
  RxTask2QHandle = osMessageQueueNew (16, sizeof(uint8_t), &RxTask2Q_attributes);

  /* creation of TxTask2Q */
  TxTask2QHandle = osMessageQueueNew (16, sizeof(uint8_t), &TxTask2Q_attributes);

  /* creation of dummyTask1Q */
  dummyTask1QHandle = osMessageQueueNew (16, sizeof(uint8_t), &dummyTask1Q_attributes);

  /* creation of dummyTask2Q */
  dummyTask2QHandle = osMessageQueueNew (16, sizeof(uint8_t), &dummyTask2Q_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of RxTask */
  RxTaskHandle = osThreadNew(fvdRxTask, NULL, &RxTask_attributes);

  /* creation of TxTask */
  TxTaskHandle = osThreadNew(fvdTxTask, NULL, &TxTask_attributes);

  /* creation of RxTask2 */
  RxTask2Handle = osThreadNew(fvdRxTask2, NULL, &RxTask2_attributes);

  /* creation of TxTask2 */
  TxTask2Handle = osThreadNew(fvdTxTask2, NULL, &TxTask2_attributes);

  /* creation of dummyTask1 */
  dummyTask1Handle = osThreadNew(fvddummyTask1, NULL, &dummyTask1_attributes);

  /* creation of dummyTask2 */
  dummyTask2Handle = osThreadNew(fvddummyTask2, NULL, &dummyTask2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  pstSerialChannel2 = pstCreateSerialChannel(&huart2, RX_BUFFER_SIZE, TX_BUFFER_SIZE,
                                             RxBufSemaHandle, TxBufSemaHandle);
  pstSerialChannel3 = pstCreateSerialChannel(&huart3, RX_BUFFER_SIZE, TX_BUFFER_SIZE,
                                             Rx2BufSemaHandle, Tx2BufSemaHandle);
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MISO_Pin|SPI1_MOSI_Pin;
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

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Audio_SCL_Pin Audio_SDA_Pin */
  GPIO_InitStruct.Pin = Audio_SCL_Pin|Audio_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_fvdRxTask */
/**
  * @brief  Function implementing the RxTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_fvdRxTask */
void fvdRxTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	  int8_t chRxChar;

	  fvdEnableSerialChInterrupts(pstSerialChannel2);
	  /* Infinite loop */
	  for(;;)
	  {
	     chRxChar = fchGetChar(pstSerialChannel2, BLOCKING);

	     osMessageQueuePut(TxTaskQHandle, &chRxChar, 0, osWaitForever);

	  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_fvdTxTask */
/**
* @brief Function implementing the TxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fvdTxTask */
void fvdTxTask(void *argument)
{
  /* USER CODE BEGIN fvdTxTask */
	  char chTxChar;
	  /* Infinite loop */
	  for(;;)
	  {
	    osMessageQueueGet(TxTaskQHandle, &chTxChar, 0, osWaitForever);

	    fuinPutChar(pstSerialChannel2, chTxChar, BLOCKING);
	  }
  /* USER CODE END fvdTxTask */
}

/* USER CODE BEGIN Header_fvdRxTask2 */
/**
* @brief Function implementing the RxTask2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fvdRxTask2 */
void fvdRxTask2(void *argument)
{
  /* USER CODE BEGIN fvdRxTask2 */
	  int8_t chRxChar;

	  fvdEnableSerialChInterrupts(pstSerialChannel3);
	  /* Infinite loop */
	  for(;;)
	  {
	     chRxChar = fchGetChar(pstSerialChannel3, BLOCKING);

	     osMessageQueuePut(TxTask2QHandle, &chRxChar, 0, osWaitForever);

	  }
  /* USER CODE END fvdRxTask2 */
}

/* USER CODE BEGIN Header_fvdTxTask2 */
/**
* @brief Function implementing the TxTask2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fvdTxTask2 */
void fvdTxTask2(void *argument)
{
  /* USER CODE BEGIN fvdTxTask2 */
	  char chTxChar;
	  /* Infinite loop */
	  for(;;)
	  {
	    osMessageQueueGet(TxTask2QHandle, &chTxChar, 0, osWaitForever);

	    fuinPutChar(pstSerialChannel3, chTxChar, BLOCKING);
	  }
  /* USER CODE END fvdTxTask2 */
}

/* USER CODE BEGIN Header_fvddummyTask1 */
/**
* @brief Function implementing the dummyTask1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fvddummyTask1 */
void fvddummyTask1(void *argument)
{
  /* USER CODE BEGIN fvddummyTask1 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END fvddummyTask1 */
}

/* USER CODE BEGIN Header_fvddummyTask2 */
/**
* @brief Function implementing the dummyTask2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fvddummyTask2 */
void fvddummyTask2(void *argument)
{
  /* USER CODE BEGIN fvddummyTask2 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END fvddummyTask2 */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
