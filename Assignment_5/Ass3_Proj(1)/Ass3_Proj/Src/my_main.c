/* Revision Information
 * ====================
 *
 * $Id: my_main.c 4868 2021-04-05 04:01:26Z rbetz $
 * $Author: rbetz $
 * $Date: 2021-04-05 14:01:26 +1000 (Mon, 05 Apr 2021) $
 * $Revision: 4868 $
 */

/*
 * Created on: Monday March 15, 2021
 * Created by: Robert Betz
 */

#include "Serial_Driver.h"
#include "my_main.h"
#include "main.h"


// Private data structures
uint8_t data[] = "Hello Cruel World\n";


stSERIAL_CHANNELTypeDef *pstSerialChannel2;

/*
 * fvdMyMain
 * =========
 *
 * This is the main program for this project. It contains virtually all of the user code in
 * the project. It is designed like this so that minimal code is present in main.c, since
 * any code there can be overwritten if the STM32Cube part of the development system
 * reconfigures the hardware.
 *
 * Parameters: none
 * Return: nothing
 */
void fvdMyMain(UART_HandleTypeDef *pstHuart2)
{
  char chRxChar;
  // Create the external USART (interrupt driven)
  pstSerialChannel2 = pstCreateSerialChannel(pstHuart2, RX_BUFFER_SIZE, TX_BUFFER_SIZE);
  fvdEnableSerialChInterrupts(pstSerialChannel2);

  while (1)
  {
    chRxChar = fchGetChar(pstSerialChannel2, NON_BLOCKING);

    if ((uint8_t)chRxChar != 255)
    {
        fuinPutChar(pstSerialChannel2, chRxChar, BLOCKING);
    }
//    fboPrintString(pstSerialChannel2, (char *)data, NON_BLOCKING);
//    HAL_Delay(250);
//    HAL_GPIO_TogglePin(GPIOD, GREEN_LED); // Green LED
  }
}


