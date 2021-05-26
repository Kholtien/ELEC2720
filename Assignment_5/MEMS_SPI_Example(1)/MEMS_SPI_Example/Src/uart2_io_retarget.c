/*
 * printf_retarget.c
 *
 *  Created on: Jan 10, 2020
 *      Author: robertbetz
 */


/*
 * This file creates two routines that re-target two of the routines that have
 * to be connected
 */

#include "main.h"

/* Set up extern for USART2 on the board */
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN 1 */
// STEPIEN: Redirect output to serial port
//          Uses blocking write to USART
//
// Added here by Bob Betz 2020
//
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)(&ch), 1, 0xFFFF);
  return 1;
}

// STEPIEN: Redirect input from serial port
//          Uses blocking read from USART
//
int __io_getchar(void)
{
  uint8_t ch;
  HAL_UART_Receive(&huart2, (uint8_t *)(&ch), 1, 0xFFFF);
  return (int)ch;
}

/* Provide access to other weak defined character input and output routines
 *  into the standard library
 */

char getchar(void)
{
	return (char)__io_getchar();
}

int putchar(int ch)
{
	return __io_putchar(ch);
}
