/*
 * printf_retarget.c
 *
 *  Created on: Jan 10, 2020
 *  Author: Bob Betz
 */


/*
 * This file creates two routines that re-target two of the routines that have
 * to be connected.
 *
 * NOTE THAT THESE ROUTINES ARE BLOCKING IN NATURE. IF THERE IS NO CHARACTER
 * ON A READ OF THE SERIAL PORT THEN THE ROUTINE BLOCKS.  SIMILARLY IF THE
 * TRANSMITTER IS BUSY THEN THE WRITE WILL BLOCK.
 */

#include "main.h"
#include "usart_io_retarget.h"

static UART_HandleTypeDef *pHuart;


/*
 * This simple routine allows the redirection of the normal
 * standard printf functions to be assigned to a huart
 * structure that is passed into this routine.
 *
 * Written by Bob Betz
 * Date: 22/10/20
 */

void my_USART_Init(UART_HandleTypeDef *pHuartAct)
{
	pHuart = pHuartAct;
}
//

/*
 * Redefinition of the weak routines to my own.
 */
int __io_putchar(int ch)

{
  HAL_UART_Transmit(pHuart, (uint8_t *)(&ch), 1, 0xFFFF);
  return 1;
}


int __io_getchar(void)
{
  uint8_t ch;
  HAL_UART_Receive(pHuart, (uint8_t *)(&ch), 1, 0xFFFF);
  return (int)ch;
}

/* Provide access to other weak defined character input and output routines
 *  into the standard library
 */

int getchar(void)
{
	return (char)__io_getchar();
}

int putchar(int ch)
{
	return __io_putchar(ch);
}
