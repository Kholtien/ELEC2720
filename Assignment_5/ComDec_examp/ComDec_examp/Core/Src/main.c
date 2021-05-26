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
#include "command_decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* Definitions for DecodingThread */
osThreadId_t DecodingThreadHandle;
const osThreadAttr_t DecodingThread_attributes = {
  .name = "DecodingThread",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t DummyThreadHandle1;
const osThreadAttr_t DummyThread1_attributes = {
  .name = "DummyThread1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t DummyThreadHandle2;
const osThreadAttr_t DummyThread2_attributes = {
  .name = "fvdDummyThread2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


/* Definitions for CharGenThread */
osThreadId_t CharGenThreadHandle;
const osThreadAttr_t CharGenThread_attributes = {
  .name = "CharGenThread",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CharGenThreadQ */
osMessageQueueId_t CharGenThreadQHandle;
const osMessageQueueAttr_t CharGenThreadQ_attributes = {
  .name = "CharGenThreadQ"
};
/* Definitions for DecodingThreadQ */
osMessageQueueId_t DecodingThreadQHandle;
const osMessageQueueAttr_t DecodingThreadQ_attributes = {
  .name = "DecodingThreadQ"
};
/* USER CODE BEGIN PV */
osMemoryPoolId_t pvdMyDynamicMemory;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void fvdDecodingThread(void *argument);
void fvdCharGenThread(void *argument);

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
  uint8_t a0chSignOnMess[] = "\r\n\r\nCommand Decoder Demo Program\r\n\r\nInput memory location to view from 20000000 to 20020000 (in hex): ";
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of CharGenThreadQ */
  CharGenThreadQHandle = osMessageQueueNew (16, sizeof(tstMessEnvelope), &CharGenThreadQ_attributes);

  /* creation of DecodingThreadQ */
  DecodingThreadQHandle = osMessageQueueNew (16, sizeof(tstMessEnvelope), &DecodingThreadQ_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of DecodingThread */
  DecodingThreadHandle = osThreadNew(fvdDecodingThread, NULL, &DecodingThread_attributes);

  /* creation of CharGenThread */
  CharGenThreadHandle = osThreadNew(fvdCharGenThread, NULL, &CharGenThread_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /*
   * Allocate a pool of Dynamic memory from the Heap
   */
  pvdMyDynamicMemory = osMemoryPoolNew(1000, 1, NULL);

  HAL_UART_Transmit(&huart2, a0chSignOnMess, sizeof(a0chSignOnMess), 20);
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

/*****************************************************************
 *
 *      TABLES AND HANDLING FUNCTIONS FOR fvdDecodingThread
 *
 * The tables and the handling functions are intermixed in this
 * section with the handling functions close to the vector table
 * that would invoke them.
 *
 *****************************************************************
 */




#define INT_ARRAY_SIZE 9 // Size of an integer location written in hex

typedef struct
    {
      uint8_t uchCharCnt;                         // Number characters in a0uchHexNum
      uint8_t a0uchHexNum[INT_ARRAY_SIZE + 1];    // Where input characters are stored.
      uint8_t uchArraySize;                       // Where the INT_ARRAY_SIZE number is stored.
      uint32_t uintAddress;                       // The converted string in binary integer
    } stCharInfo_Type;



/*====================================================================================*/
// ASCII number input handler
/*
 * fnAssum_AsciiNum - DT0
 *
 * This function accepts takes ASCII number character passed in and echoes it
 * to the terminal, and then inputs it into a string that will later be
 * converted into a floating point number.
 *
 * The string is passed in via the info_struc_ptr component of the
 * dec_input_struc.
 *
 * The table pointers remain unaltered for this routine.
 *
 * Param: dec_input_struc *
 *
 * Return: void
 */
void fvdAccum_AsciiNum(dec_input_struc *pstDecoderStruc)
{
  uint8_t uchInputChar = pstDecoderStruc->data_byte;
  stCharInfo_Type * pstCharInfo;

  /* Check to see if the array length is going to be exceeded
   * if we put the new character into the array.
   */
  pstCharInfo = (stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr);

  if ( pstCharInfo->uchCharCnt < pstCharInfo->uchArraySize - 1)
  {
    pstCharInfo->a0uchHexNum[pstCharInfo->uchCharCnt++] = uchInputChar;
    // Echo to the terminal
    HAL_UART_Transmit(&huart2, &uchInputChar, 1, 10);
  }
}

/*====================================================================================*/

/*
 * fvdCR_received - DT0
 *
 * This handler is entered if a CR has been input by the user. This is assumed to be
 * the indication of the termination of an number input.
 *
 * If there is a number has accumulated in the stCharInfo_Type data structure
 * pointed to by the pstDecoderStruc->info_struc_ptr then this is converted from
 * ascii hexadecimal to binary.  Then the contents of the memory location pointed
 * to by the resultant address is presented on the screen.
 *
 * Param: dec_input_struc *
 *
 * Returns: void
 *
 */
void fvdCR_received(dec_input_struc *pstDecoderStruc)
{
  uint8_t *puchCharCnt;
  uint32_t *puinMemLoc;

  char a20chReturned_ascii[20];

  // Messages
  char a0chMess1[] =
      {
          "Contents of memory location are: "
      };
  char a0chMess2[] =
      {
          "No characters input. Try again\r\n"
      };

  char a0chMess3[] =
      {
          "Illegal memory range. Try again\r\n"
      };

  char a0chMess4[] =
      {
          "Input a new memory address:\r\n"
      };

  // Write CRLF to the terminal
  HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 10);

  puchCharCnt = &(((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->uchCharCnt);

  if (*puchCharCnt != 0)
  {
    // There are ascii characters in the buffer.
    // Convert the buffer contents to a string
    ((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->a0uchHexNum[*puchCharCnt] = '\0';

    // Now convert ascii hex number to an binary integer
    puinMemLoc = (uint32_t*)strtol((char *)((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->a0uchHexNum,
                                                                   NULL, 16);
    ((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->uintAddress = (uint32_t)puinMemLoc;

    if (((uint32_t)puinMemLoc >= 0x20000000) && ((uint32_t)puinMemLoc < 0x20020000))
    {
      /*
       * !!!!! DON'T USE THE SPRINTF FUNCTION. !!!!!
       *
       * It corrupts the software as it uses the malloc routine somewhere
       * under the hood. This is not consistent with FreeRTOS.
       */
      // sprintf(a20chReturned_ascii, "%lx", (uint32_t)puinMemLoc);

      itoa((uint32_t)*puinMemLoc, a20chReturned_ascii, 16);
      strcat(a20chReturned_ascii, "\r\n");

      HAL_UART_Transmit(&huart2, (uint8_t*)a0chMess1, strlen(a0chMess1), 10);
      HAL_UART_Transmit(&huart2, (uint8_t*)a20chReturned_ascii, strlen(a20chReturned_ascii), 10);
      HAL_UART_Transmit(&huart2, (uint8_t*)"\r\nInput a new memory location in hex: ",
                                    strlen("\r\nInput a new memory location in hex: "), 10);
    }
    else
    {
      HAL_UART_Transmit(&huart2, (uint8_t*)a0chMess3, strlen(a0chMess3), 10);
      HAL_UART_Transmit(&huart2, (uint8_t*)a0chMess4, strlen(a0chMess4), 10);

    }
  }
  else
  {
    // No valid characters have been input
    HAL_UART_Transmit(&huart2, (uint8_t*)a0chMess2, strlen(a0chMess2), 10);
  }
  // HAL_UART_Transmit(&huart2, (uint8_t*)"Got to CR handler", strlen("Got to CR handler"), 10);
  *puchCharCnt = 0;
}

/*==========================================================================================*/

/*
 * fvdBS_received
 *
 * Simply deletes a character if there is a character to be deleted.
 *
 */
void fvdBS_received(dec_input_struc *pstDecoderStruc)
{
  uint16_t uchCharCnt;

  // The user has request that the previous character be deleted.
  uchCharCnt = ((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->uchCharCnt;

  if (uchCharCnt > 0)
  {
    (((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->uchCharCnt)--;
    HAL_UART_Transmit(&huart2, (uint8_t*)"\b \b", 3, 10);
  }
}


/*==========================================================================================*/

/*
 * fvdDEL_received
 *
 * Simply deletes a character if there is a character to be deleted.
 *
 */
void fvdDEL_received(dec_input_struc *pstDecoderStruc)
{
  uint8_t uchCharCnt;

  // The user has request that the previous character be deleted.
  uchCharCnt = ((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->uchCharCnt;

  if (uchCharCnt > 0)
  {
    (((stCharInfo_Type *)(pstDecoderStruc->info_struc_ptr))->uchCharCnt)--;
    HAL_UART_Transmit(&huart2, (uint8_t*)"\b \b", 3, 10);
  }
}


/*==========================================================================================*/

/*
 * fvdInvalidChar - DT0
 *
 * This function is entered if the user types and invalid character in a particular input table
 * Does not echo the character and returns to the same set of tables.
 *
 */
void fvdInvalid_Char(dec_input_struc *pstDecoderStruc)
{
  ;
}



/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*
 * Initial Decoding Tables
 */
// For the valid sending task CharGenThread
uint8_t a10uchVdt_DT0[10] =
                        {
                          CHAR_RANGE('0', '9'),
                          CHAR_RANGE('a', 'f'),
                          CHAR_RANGE('A', 'F'),
                          CR_CHAR,
                          BS_CHAR,
                          DEL_CHAR,
                          ASC_TABLE_END
                        };

void(*a7pfvdVt_DT0[7])(dec_input_struc *) =
                          {
                            fvdAccum_AsciiNum,
                            fvdAccum_AsciiNum,
                            fvdAccum_AsciiNum,
                            fvdCR_received,
                            fvdBS_received,
                            fvdDEL_received,
                            fvdInvalid_Char
                          };

// For an invalid sending task
uint8_t a1uchVdt_DT0[1] =
                        {
                            ASC_TABLE_END
                        };

void(*a1pfvdVt_DT0[1])(dec_input_struc *) =
                          {
                            fvdInvalid_Char
                          };


/* USER CODE END 4 */



/* USER CODE BEGIN Header_fvdDecodingThread */



/*===========================================================================*/


/**
  * @brief  Function implementing the DecodingThread thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_fvdDecodingThread */
void fvdDecodingThread(void *argument)
{
  /* USER CODE BEGIN 5 */

  addr_struc stDecTaskAddrStruc;

  stCharInfo_Type stInputDataStore;

  // Delare the dec_input_struc that will be used for decoding the
  // different valid sending tasks.
  dec_input_struc stDecodingTaskDecStruc;         // Decoding table for the CharGenThread sender task
  dec_input_struc stDecodingTaskAddrERROR;        // Decoding table for the sender task error -- i.e. wrong sender.

  // Allocate valid tasks table
  osMessageQueueId_t pvdValidAddr_DecodingThread[2];

  // Initialise valid tasks table.
  pvdValidAddr_DecodingThread[0] = CharGenThreadQHandle;    // CharGenThread Q handle
  pvdValidAddr_DecodingThread[1] = NULL;                    // Termination NULL

  // Associated decode structures for the sending tasks
  dec_input_struc *pstSenderDecStruc[2] =
        {
            &stDecodingTaskDecStruc,
            &stDecodingTaskAddrERROR
        };



  /*
   * Initialise the dec_input_struc with initial decode tables
  */
  // Firstly the table for the valid sending task CharGenThread
  stDecodingTaskDecStruc.char_dec_mode = ASCII_DECODING;
  stDecodingTaskDecStruc.valid_data_table_ptr = a10uchVdt_DT0;
  stDecodingTaskDecStruc.vector_table_ptr = a7pfvdVt_DT0;
  stDecodingTaskDecStruc.bit_table_ptr = NULL;
  stDecodingTaskDecStruc.info_struc_ptr = (void*)&stInputDataStore;

  // Now the initial the table if there is an error in the sending task
  stDecodingTaskAddrERROR.char_dec_mode = ASCII_DECODING;
  stDecodingTaskAddrERROR.valid_data_table_ptr = a1uchVdt_DT0;
  stDecodingTaskAddrERROR.vector_table_ptr = a1pfvdVt_DT0;
  stDecodingTaskAddrERROR.bit_table_ptr = NULL;

  /*
   * Initialise components of the valid sending task address structure
   */
  stDecTaskAddrStruc.pvdTaskQueueId = DecodingThreadQHandle;
  stDecTaskAddrStruc.valid_addr_tab_ptr = pvdValidAddr_DecodingThread;
  stDecTaskAddrStruc.addr_vector_ptr = pstSenderDecStruc;
  stDecTaskAddrStruc.pvdMemoryPoolId = pvdMyDynamicMemory;
  stDecTaskAddrStruc.info_struc_ptr = (void*)&stInputDataStore;

  /*
   * Initialise the structure used to accumulate the user input characters
   */
  stInputDataStore.uchCharCnt = 0;
  stInputDataStore.uchArraySize = INT_ARRAY_SIZE;


  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
    // Now call the decoder with the relevant data structures
    decode_mbx_mess(&stDecTaskAddrStruc, osWaitForever);
  }
  /* USER CODE END 5 */
}


void DummyThread1(void *argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

void DummyThread2(void *argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/*===========================================================================*/



/* USER CODE BEGIN Header_fvdCharGenThread */
/**
* @brief Function implementing the CharGenThread thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_fvdCharGenThread */
void fvdCharGenThread(void *argument)
{
  /* USER CODE BEGIN fvdCharGenThread */
  tstMessEnvelope stMyEnvelope;
  uint8_t chCharData[2];

  /*
   * Now set up some static values in the envelope
   */
  stMyEnvelope.pvdRecipientQueueId = DecodingThreadQHandle;
  stMyEnvelope.pvdSendTaskQueueId = CharGenThreadQHandle;
  stMyEnvelope.uchDynamicMemFlag = TRUE;
  stMyEnvelope.suinMessSize = 1;
  /* Infinite loop */
  for(;;)
  {
    HAL_UART_Receive(&huart2, chCharData, 1, HAL_MAX_DELAY);

    /*
     * Have now received a character so send it to the decoding task.
     *
     * Firstly allocated a byte of memory for the message, assign
     * the received byte to it, place the pointer in the envelope.
     */
    stMyEnvelope.puchMess = (uint8_t*)osMemoryPoolAlloc(pvdMyDynamicMemory, osWaitForever);

    // Now copy the received character into the newly allocated memory
    *(stMyEnvelope.puchMess) = chCharData[0];

    osMessageQueuePut( stMyEnvelope.pvdRecipientQueueId, &stMyEnvelope, 0, osWaitForever);
  }
  /* USER CODE END fvdCharGenThread */
}


/*===========================================================================*/



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
