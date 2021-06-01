/* Revision Information
 * ====================
 *
 * $Id: Serial_Driver.c 5025 2021-05-26 12:07:07Z rbetz $
 * $Author: rbetz $
 * $Date: 2021-05-26 22:07:07 +1000 (Wed, 26 May 2021) $
 * $Revision: 5025 $
 */


/*
 * ***********************************************************
 *
 *                  SERIAL DRIVER MODULE
 *
 * Created: October 28, 2020
 * Created by: Bob Betz
 *
 * BASIC DESCRIPTION
 *  -----------------
 *
 * This file contains the code for interrupt serial drivers
 * for the Nucleo-F767ZI processor board by STM.
 *
 * These drivers assume that the initial project for the
 * software has been set up using STM32CubeIDE, with the
 * relevant USART/s enabled and the interrupts enable for
 * them. This initial setup will initialise the internal
 * NVIC with appropriate initialisation, as well as the
 * basic HAL STM32 data structures required for the USART.
 *
 * Some of the data structures created by STM32CubeIDE will
 * be used in the interrupt serial driver routines.  However
 * used at all. Most of the HAL routines for the interrupts
 * are not used in this code. These routines are large, and
 * consequently would be quite slow in execution.  Hopefully
 * these serial routines are a lot lighter and therefore
 * faster.  In addition these routines implement proper
 * circular buffers, as compared to the HAL routines whose
 * buffers are not circular.
 *
 *
 *  Capabilities
 *    ------------
 *
 * This serial driver is set up so that multiple serial
 * channels can be formed using its code -- the code is
 * fully reentrant.
 *
 * Each of the serial channels does assume that the serial
 * data is ASCII data (not binary). Therefore the normal
 * interpretations of ASCII serial data can be used on
 * data received over the channel.
 *
 * All of the channels created obey the Ctrl-S, Ctrl-Q for
 * software handshaking and flow control. This operates both
 * on the RX channel if the RX buffer reaches a full threshold,
 * and also on the TX buffer is the channel external client
 * wishes to stop the data being transmitted from this
 * microcontroller.
 *
 *
 * USING THE SERIAL DRIVER
 *  -----------------------
 *
 * In order to connect to the serial driver in this module
 * one has to modify the code produced by STM32CubeIDE so
 * that the normal interrupt STM32CubeIDE routine diverts the code
 * execution to the interrupt routines in this module. This
 * is achieved by the modifying the following routine
 * (in this case for USART2) shown below. This routine is
 * located in the auto-generated file
 *
 * stm32f7xx_it.c
 *
 *
 * void USART2_IRQHandler(void)
 *   {
 *      HAL_UART_IRQHandler(&huart2);
 *
 *   }
 *
 * to
 *
 * void USART2_IRQHandler(void)
 *   {
 *     fvdSerialChannelISR(pstSerialCh2);
 *   }
 *
 * where fnIrqSerialChannel(pstSerialCh2) is the function
 * that is the serial channel interrupt routine.
 *
 * NOTE: This assumes the the serial channel has been created
 * by the STM32CubeIDE system initialisation code.
 *
 *
 * Variable Initialisation
 *    -----------------------
 *
 *In order to use the serial channel interrupt system it is
 *necessary to create the channel first. The process will
 *create all of the significant data structures used by the
 *serial channel.  These data structures are created
 *dynamically and are accessed via a global variable
 *
 *stSERIAL_CHANNEL * pstUsartx;
 *
 *which contains all of the information for the receive and
 *transmit channels. This data structure and all of the
 *others are defined in Serial_Driver.h.  Call pstCreateSerialChannel
 *to create each serial channel.
 *
 *The header file Serial_Driver.h must be included into main.c.
 *
 *NOTE: AS noted above, the routines in this module are totally
 *reentrant. Therefore there are no global variables used in this module
 *and no local static variables.
 *
 *
 */
#include "Serial_Driver.h"
#include <math.h>

/*
 * void fnvdSerialChannelISR(stSERIAL_CHANNELTypeDef *pstSerialCh)
 * ---------------------------------------------------------------
 *
 * This is the generic interrupt service routine (ISR) that is called if
 * there is an interrupt on a particular channel.  The interrupt can
 * either be a TX or Rx interrupt.
 *
 * For this routine to be called a manual modification of the
 * code produced by STM32CubeIDE has to be made. As mentioned in
 * the general description, STM32CubeIDE generates specific interrupt
 * routines for each of the USARTs that have their interrupts enabled.
 * These routines have the generic name:
 *
 * void USARTx_IRQHandler(void)
 *   {
 *     HAL_UART_IRQHandler(&huartx);
 *
 *   }
 *
 * where the 'x' corresponds to the interrupt number. The
 * HAL_UART_IRQHandler(&huartx) in this routine has to be replaces
 * with this routine, so that we have
 *
 * void USARTx_IRQHandler(void)
 *   {
 *      fnvdSerialChannelISR(pstSerialChx);
 *   }
 *
 * This routine basically checks to see what the source of the
 * interrupt is for the particular UART and then services that interrupt.
 *
 * In order to do the servicing many of the details for the particular
 * USART are in the STM32CubeIDE generated data structure huartx. This
 * data structure is available in the pstSerialCh data structure passed
 * into this function.
 *
 * Clearly to be entering this routine interrupts for the serial channel
 * must be enabled.
 *
 */

// Rx interrrupt counter
uint16_t suinRxIntCounter = 0;

void fvdSerialChannelISR(stSERIAL_CHANNELTypeDef *pstSerialCh)
{
  // Firstly read in some relevant registers
  
  uint32_t srflags = READ_REG(pstSerialCh->pstRxChannel->pstUartHALHandle->Instance->SR); // Status Register
  uint32_t cr1its = READ_REG(pstSerialCh->pstRxChannel->pstUartHALHandle->Instance->CR1); // Control register 1
  // uint32_t cr3its = READ_REG(pstSerialCh->pstRxChannel->pstUartHALHandle->Instance->CR3);    // Control register 3

  uint32_t ulgErrorflags;
  enum BOOL boInterruptOccurred = False;

  // Firstly check to see if there are any errors. This code is basically the same.
  ulgErrorflags = (srflags &(uint32_t)(USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE ));

  if (ulgErrorflags == 0U)
  {
    // No errors then continue.

    // Check for the source of the interrupt
    // Check to see if Rx interrupts are enabled and if a Rx interrupt has actually occurred.
    if (((srflags & USART_SR_RXNE) != 0U) && ((cr1its & USART_CR1_RXNEIE) != 0U))
    {
      // Call the receive interrupt handling routine.
      fvdRxInterruptHandler(pstSerialCh);
      boInterruptOccurred = True;
    }

    // Now check to see if an Tx interrupt has occurred.
    if (((srflags & USART_SR_TXE) != 0U) && ((cr1its & USART_CR1_TXEIE) != 0U))
    {
      fvdTxInterruptHandler(pstSerialCh);
      boInterruptOccurred = True;
    }
  } // ulgErrorflags == 0

  // Save the error flags for perusal under debugging
  pstSerialCh->ulSerialErrorFlags = ulgErrorflags;

  // Now check to make sure that at least one Rx or TX interrupt has been serviced
  if (!boInterruptOccurred)
  {
    pstSerialCh->ulSerialErrorCodes |= INTERRUPT_GEN_ERROR;
    // Clear any interrupts to prevent the routine from getting caught in an
    // interrupt loop.
    HAL_NVIC_ClearPendingIRQ(pstSerialCh->uinIRQn);
  }
} // end fnvdSerialChannelISR

/*
 * void fvdRxInterruptHandler(stSERIAL_CHANNELTypeDef *pstSerialCh)
 * ----------------------------------------------------------------
 *
 * This is the actual receive interrupt handler.  It is called from the generic
 * interrupt routine if the Rx interrupt is one of the interrupts. This routine
 * does most of the work with respect to receive interrupts.
 *
 * This routine is only entered if there are no errors, and there is a valid
 * receiver not empty interrupt.
 *
 * The receiver handles the CtrlS/Q protocol with respect to the receive buffer
 * filling up, and also explicit CtrlS/Q received from the serial channel remote
 * user.  In this case it interacts with the transmit channel via a flag in the
 * transmit buffer.
 *
 */

void fvdRxInterruptHandler(stSERIAL_CHANNELTypeDef *pstSerialCh)
{
  char chTempCh;
  // The first step is to read in the value in the RxDR
  chTempCh = pstSerialCh->pstRxChannel->pstUartHALHandle->Instance->DR;

  // After a certain number of Rx interrupts reset the RED overflow led so
  // that one can see if overflows are continuing.
  if (((suinRxIntCounter++) % RX_BUFFER_SIZE) == 0)
  {
   //  HAL_GPIO_WritePin(GPIOD, RED_LED, GPIO_PIN_RESET);
    suinRxIntCounter = 0;
  }

  switch (chTempCh)
  {
    case CTRLS:
      // The external user has requested that the Tx be stopped.
      // When this is received a location is set in the transmit
      // structure to indicate that the transmission data should stop upon
      // the next TX interrupt.
      DISABLE_TXE_INT(pstSerialCh->pstTxChannel);
      pstSerialCh->pstTxChannel->boTxUserCtrlS = True;
      return;

    case CTRLQ:
      // The external user has requested that the Tx transmission be started
      // again
      pstSerialCh->pstTxChannel->boTxUserCtrlS = False;
      ENABLE_TXE_INT(pstSerialCh->pstTxChannel);
      return;
  } // end switch

  // Enter here if no user CtrlS or Ctrl Q, so normal receive character processing
  // should proceed.
  //
  // Prior to putting the character into the buffer check to see if the buffer is
  // full.
  if (pstSerialCh->pstRxChannel->uinFree == 0)
  {
    // Buffer is full.  This condition should not occur and there is an error
    // if it has occurred.
    // Throw away the character
    pstSerialCh->ulSerialErrorCodes |= RX_BUFFER_FULL;
    // HAL_GPIO_WritePin(GPIOD, RED_LED, GPIO_PIN_SET);

    // Clear RXNE interrupt flag
    // __HAL_UART_SEND_REQ(pstSerialCh->pstRxChannel->pstUartHALHandle, UART_RXDATA_FLUSH_REQUEST);
    return;
  }

  // Get here if there is space available in the buffer.
  // Put the character into the buffer and update the house keeping
  // values
  pstSerialCh->pstRxChannel->puchSerialBuffer[pstSerialCh->pstRxChannel->uinHead] = chTempCh;
  pstSerialCh->pstRxChannel->uinHead = (pstSerialCh->pstRxChannel->uinHead + 1) %
                                                    (pstSerialCh->pstRxChannel->uinBufSize);
  pstSerialCh->pstRxChannel->uinUsed++;
  pstSerialCh->pstRxChannel->uinFree--;

  // Now check to see if the upper threshold has been exceeded
  if (pstSerialCh->pstRxChannel->uinUsed >= pstSerialCh->pstRxChannel->uinCtrlSThreshold)
  {
    // We are at or above the upper threshold for used locations in the buffer.
    // So instruct the TX to send out a CtrlS to stop characters.
    pstSerialCh->pstRxChannel->chCtrlSCtrlQ = CTRLS;

    // Set the flag to indicate to the Rx routine that it is in hysteresis mode
    // and waiting for the buffer to be depleted before a CtrlQ is sent out of
    // the Tx
    pstSerialCh->pstRxChannel->boHysteresisActive = True;
    // Make sure the the TX interrupts are enable so that this can be checked by the
    // transmitter and the ctrlS is sent.
    ENABLE_TXE_INT(pstSerialCh->pstTxChannel);
  }
#ifdef __FREERTOS__
  osSemaphoreRelease(pstSerialCh->pstRxChannel->pvdRxSemaId);
#endif
} // end fvdRxInterruptHandler

/*
 * void fvdTxInterruptHandler(stSERIAL_CHANNELTypeDef *pstSerialCh)
 * ----------------------------------------------------------------
 *
 * This is the actual transmit interrupt handler.  It is called from the generic
 * interrupt routine if the Tx interrupt is one of the interrupts. This routine
 * does most of the work with respect to transmit interrupts.
 *
 * This routine is only entered if there are no errors, and there is a valid
 * transmitter empty interrupt.
 *
 * The transmitter handles the CtrlS/Q protocol with respect to the receive buffer
 * filling up, and also explicit CtrlS/Q received from the Rx serial channel remote
 * user.  In this case it interacts with the Rx channel via a flag in the
 * transmit buffer.
 *
 */

void fvdTxInterruptHandler(stSERIAL_CHANNELTypeDef *pstSerialCh)
{
  // Upon entry firstly check to see if the receiver has requested that the
  // transmitter transmit a CtrlS or CtrlQ character to control the filling
  // of the RX buffer.
  //
  // We have to check the chCtrlSCtrlQ flag in the RX data structure
  if ((pstSerialCh->pstRxChannel->chCtrlSCtrlQ == CTRLS) || (pstSerialCh->pstRxChannel->chCtrlSCtrlQ == CTRLQ))
  {
    // Now send the CTRLS/Q immediately out of the transmit register of the
    // UART, and then reset the chCtrlSCtrlQ location to zero.
    pstSerialCh->pstTxChannel->pstUartHALHandle->Instance->DR = pstSerialCh->pstRxChannel->chCtrlSCtrlQ;
/*

    if (pstSerialCh->pstRxChannel->chCtrlSCtrlQ == CTRLS)
    {
      HAL_GPIO_WritePin(GPIOD, ORANGE_LED, GPIO_PIN_SET);
    }
    else if (pstSerialCh->pstRxChannel->chCtrlSCtrlQ == CTRLQ)// CTRLQ
    {
      HAL_GPIO_WritePin(GPIOD, ORANGE_LED, GPIO_PIN_RESET);
    }

*/
    pstSerialCh->pstRxChannel->chCtrlSCtrlQ = 0;
    return;
  }

  if (pstSerialCh->pstTxChannel->uinUsed == 0)
  {
    DISABLE_TXE_INT(pstSerialCh->pstTxChannel);
    return;
  }

  if (pstSerialCh->pstTxChannel->boTxUserCtrlS)
  {
    // The external user has requested that the serial transmission stop
    DISABLE_TXE_INT(pstSerialCh->pstTxChannel);
    return;
  }

  // Now get a character out of the transmit buffer and send it
  pstSerialCh->pstTxChannel->pstUartHALHandle->Instance->DR =
    (uint8_t)((pstSerialCh->pstTxChannel->puchSerialBuffer[pstSerialCh->pstTxChannel->uinTail]) & 0x7FU);
  // Update the buffer information
  pstSerialCh->pstTxChannel->uinTail = (pstSerialCh->pstTxChannel->uinTail + 1) %
    (pstSerialCh->pstTxChannel->uinBufSize);
  pstSerialCh->pstTxChannel->uinFree++;
  pstSerialCh->pstTxChannel->uinUsed--;
#ifdef __FREERTOS__
  osSemaphoreRelease(pstSerialCh->pstTxChannel->pvdTxSemaId);
#endif
} // end fvdTxInterruptHandler


//=================================================================================================

/*
 * USER INTERFACE FUNCTIONS
 *
 * This section of the module contains the functions that the user calls to interact with the
 * serial interrupt channels. These functions fall into three main categories:
 *
 * (i) Initialisation and interrupt control routines.
 *
 * (ii) The functions that allow one to put data into, and to take data out of the serial channels
 *
 * (iii) Various functions to see how much of the buffer space has been used in the
 * Rx and Tx channels.
 *
 * The functions to put data into or take data out of he buffers can be of two types:
 *
 * (i) Blocking -- they wait for data to arrive or for room to be available in the buffers
 *
 * (ii) Non-blocking -- if the buffer does not have room or data is not available then the
 *     function call returns immediately.
 *
 */

/*
 * stSERIAL_CHANNELTypeDef* pstCreateSerialChannel(UART_HandleTypeDef *pstHuart,
 *                  uint16_t uinRxBufSize, uint16_t uinTxBufSize)
 * ------------------------------------------------------------------------------
 *
 * This function creates the serial channel. It's
 * primary function is to create the data structures required
 * for the serial channel system.
 *
 * In order to achieve some hysterisis on the CtrlS/Q signalling the uinRxBufSize
 * should be larger than 10 characters.
 *
 * Returns : Pointer to stSERIAL_CHANNEL if successful, else a NULL pointer
 *
 * REB 26/05/21
 * Added the MEMORY_ALLOC macro so that FreeRTOS and non-FreeRTOS applications
 * can be catered for. FreeRTOS cannot use the normal malloc and calloc routines
 * in the GNU C library.
 *
 * Also added code, invoked if __FREERTOS__ is defined to allow the creation of
 * the resource counting semaphores when operating under the FreeRTOS system.
 */

stSERIAL_CHANNELTypeDef* pstCreateSerialChannel(
                                               UART_HandleTypeDef *pstHuart,
                                               uint16_t uinRxBufSize,
                                               uint16_t uinTxBufSize
#ifdef __FREERTOS__
                                               ,
                                               osSemaphoreId_t pvdRxSemaId,
                                               osSemaphoreId_t pvdTxSemaId

#endif
                                              )
{
  uint8_t* puchRxBufTmp;
  uint8_t* puchTxBufTmp;
  stRX_SERIAL_CHANNELTypeDef* pstRX_Serial_Channel;
  stTX_SERIAL_CHANNELTypeDef* pstTX_Serial_Channel;
  stSERIAL_CHANNELTypeDef* pstSerialChannel;

  // First step is to create the Rx and Tx structures.
  // First allocate the buffers
  // used for the Rx and Tx channels
  if ((puchRxBufTmp = (uint8_t*)MEMORY_ALLOC(uinRxBufSize)) == NULL)
  {
    // Not enough memory so return
    return (stSERIAL_CHANNELTypeDef*) puchRxBufTmp;
  }

  if ((puchTxBufTmp = (uint8_t*)MEMORY_ALLOC(uinTxBufSize)) == NULL)
  {
    // Not enough memory so return
    return (stSERIAL_CHANNELTypeDef*) puchTxBufTmp;
  }

  // Get here if the buffers have been allocated correctly.
  // Now allocate the storage for the main structures

  // RX data structure
  if ((pstRX_Serial_Channel = (stRX_SERIAL_CHANNELTypeDef*)MEMORY_ALLOC(sizeof(stRX_SERIAL_CHANNELTypeDef))) == NULL)
  {
    return (stSERIAL_CHANNELTypeDef*) pstRX_Serial_Channel;
  }

  // TX data structure
  if ((pstTX_Serial_Channel = (stTX_SERIAL_CHANNELTypeDef*)MEMORY_ALLOC(sizeof(stTX_SERIAL_CHANNELTypeDef))) == NULL)
  {
    return (stSERIAL_CHANNELTypeDef*) pstTX_Serial_Channel;
  }

  // Now fill in the data structure.
  // NOTE: The basic asynchronous communication parameters are in the *pstHuart structure and do not have to
  // be set up. They have been set up and programmed into the hardware by the HAL initialisation.

  // Firstly set up the RX data structure
  pstRX_Serial_Channel->pstUartHALHandle = pstHuart;
  pstRX_Serial_Channel->uinHead = 0;
  pstRX_Serial_Channel->uinTail = 0;
  pstRX_Serial_Channel->uinFree = uinRxBufSize;
  pstRX_Serial_Channel->uinUsed = 0;
  pstRX_Serial_Channel->uinBufSize = uinRxBufSize;
  pstRX_Serial_Channel->chCtrlSCtrlQ = 0;
  pstRX_Serial_Channel->boHysteresisActive = False;
  pstRX_Serial_Channel->uinCtrlSThreshold = RX_CTRLS_LEVEL;
  pstRX_Serial_Channel->uinCtrlQThreshold = RX_CTRLQ_LEVEL;
  pstRX_Serial_Channel->boOverflowErr = False;
  pstRX_Serial_Channel->boParityErr = False;
  pstRX_Serial_Channel->puchSerialBuffer = puchRxBufTmp;
#ifdef __FREERTOS__
  pstRX_Serial_Channel->pvdRxSemaId = pvdRxSemaId;
#endif

  // Now set up the TX data structure
  pstTX_Serial_Channel->pstUartHALHandle = pstHuart;
  pstTX_Serial_Channel->uinHead = 0;
  pstTX_Serial_Channel->uinTail = 0;
  pstTX_Serial_Channel->uinFree = uinTxBufSize;
  pstTX_Serial_Channel->uinUsed = 0;
  pstTX_Serial_Channel->uinBufSize = uinTxBufSize;
  pstTX_Serial_Channel->boTxUserCtrlS = False;
  pstTX_Serial_Channel->puchSerialBuffer = puchTxBufTmp;
#ifdef __FREERTOS__
  pstTX_Serial_Channel->pvdTxSemaId = pvdTxSemaId;
#endif

  // Serial Channel data structure
  if ((pstSerialChannel = (stSERIAL_CHANNELTypeDef*) MEMORY_ALLOC(sizeof(stSERIAL_CHANNELTypeDef))) == NULL)
  {
    return pstSerialChannel;
  }

  // Now assign the pointers to the RX and TX structure in the stSERIAL_CHANNELTypeDef structure
  pstSerialChannel->pstRxChannel = pstRX_Serial_Channel;
  pstSerialChannel->pstTxChannel = pstTX_Serial_Channel;

  // Now find out which USART or UART that we are setting up
  // This particular processor has USARTs 1 -> 3 and UARTs 4->6. These USARTs
  // each have a different register base address that is defined in stm32f767xx.h
  // of the form USART1, USART2, USART3 etc. Therefore in order to find out
  // which UART we are dealing with and what the interrupt is for this we
  // can simply check the base address.
  if (pstHuart->Instance == USART1)
    pstSerialChannel->uinIRQn = USART1_IRQn;
  else if (pstHuart->Instance == USART2)
    pstSerialChannel->uinIRQn = USART2_IRQn;
  else if (pstHuart->Instance == USART3)
    pstSerialChannel->uinIRQn = USART3_IRQn;
  else if (pstHuart->Instance == UART4)
    pstSerialChannel->uinIRQn = UART4_IRQn;
  else if (pstHuart->Instance == UART5)
    pstSerialChannel->uinIRQn = UART5_IRQn;
  else if (pstHuart->Instance == USART6)
    pstSerialChannel->uinIRQn = USART6_IRQn;
  else
    // Problem - instance does not make sense - does not exist
    return (stSERIAL_CHANNELTypeDef*) NULL;

  // Initialise to no errors.
  pstSerialChannel->ulSerialErrorFlags = 0U;

  // Initialise the error codes equal to zero
  pstSerialChannel->ulSerialErrorCodes = 0U;

  // RED LED used to indicate Rx Buffer overflow
  // HAL_GPIO_WritePin(GPIOD, RED_LED, GPIO_PIN_RESET);

  // ORANGE LED toggles on with CTRL S, and off with CTRL Q
  // HAL_GPIO_WritePin(GPIOD, ORANGE_LED, GPIO_PIN_RESET);

  // Now return a pointer to the filled in structure.
  return pstSerialChannel;

}

/*
 * void fvdEnableSerialChInterrupts(stSERIAL_CHANNELTypeDef* pstSerialCh)
 * --------------------------------------------------------
 *
 * This function enables all of the interrupts for a serial
 * channel -- i.e. it enables RX and TX for the particular
 * interrupt, and globally enables interrupts in the NVIC.
 */

void fvdEnableSerialChInterrupts(stSERIAL_CHANNELTypeDef *pstSerialCh)
{
  // Enable RX not empty interrupt
  ENABLE_RXNE_INT(pstSerialCh->pstRxChannel);
  /* Enable the UART Parity Error Interrupt */
  // __HAL_UART_ENABLE_IT(pstSerialCh->pstRxChannel->pstUartHALHandle, UART_IT_PE);

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  // __HAL_UART_ENABLE_IT(pstSerialCh->pstRxChannel->pstUartHALHandle, UART_IT_ERR);

  /* Enable the UART Data Register not empty Interrupt */
  // __HAL_UART_ENABLE_IT(pstSerialCh->pstRxChannel->pstUartHALHandle, UART_IT_RXNE);

  // Enable TX empty interrupt
  // SET_BIT(pstSerialCh->pstTxChannel->pstUartHALHandle->Instance->CR1, USART_CR1_TXEIE);
  // ENABLE_TXE_INT(pstSerialCh->pstTxChannel->pstUartHALHandle->Instance->CR1);

  // Globally enable interrupts for this USART. Not necessary as enabled in HAL_UART_MspInit
  // in the normal initialisation procedures.
  HAL_NVIC_EnableIRQ(pstSerialCh->uinIRQn);
}

/*
 * char fchGetChar(stSERIAL_CHANNELTypeDef* pstSerialCh, BOOL boBlocking)
 * ----------------------------------------------------------------------
 * 
 * This function is called to retrieve a character from the serial channel.
 * If the channel is non-blocking, and if there is no character in the
 * buffer then the routine returns 255, else it will return a valid ascii
 * character.
 *
 * If the routine is blocking then it will wait until a character arrives.
 *
 * Returns: An ascii character if in blocking mode, or an ascii character
 * if one in the buffer upon a call, or the integer 255 if there is no
 * character in the buffer.
 *
 */

char fchGetChar(stSERIAL_CHANNELTypeDef *pstSerialCh, enum BOOL boBlocking)
{
  char chTempChar;
#ifdef __FREERTOS__
  if (!boBlocking)
  {
    // Check here to see if the semaphore is non-zero or not
    if (osSemaphoreGetCount(pstSerialCh->pstRxChannel->pvdRxSemaId) == 0)
    {
      return 255;
    }
  }
  osSemaphoreAcquire(pstSerialCh->pstRxChannel->pvdRxSemaId, osWaitForever);
#else
  // not __FREERTOS__
  if (!boBlocking)
  {
    // Enter here is non-blocking. Check to see if data is there. If
    // not then return with a NON-ASCII character.

    if (pstSerialCh->pstRxChannel->uinUsed == 0)
    {
      return 255;
    }
  }

  // Enter here if there is data (non-blocking), or regardless of if there
  // is data or not for non-blocking.
  // Loop until data is available
  while (pstSerialCh->pstRxChannel->uinUsed == 0)
  {;
  }
#endif



  // There is data in the buffer so get a character out and update all
  // of the housekeeping information

  // !!!!!! START OF CRITICAL SECTION !!!!!!
#ifdef __FREERTOS__
  __disable_irq();
#else
  HAL_NVIC_DisableIRQ(pstSerialCh->uinIRQn);
#endif
  //DISABLE_RXNE_INT(pstSerialCh->pstRxChannel);
  chTempChar = pstSerialCh->pstRxChannel->puchSerialBuffer[pstSerialCh->pstRxChannel->uinTail];
  pstSerialCh->pstRxChannel->uinTail = (pstSerialCh->pstRxChannel->uinTail + 1) %
                                                      (pstSerialCh->pstRxChannel->uinBufSize);
  pstSerialCh->pstRxChannel->uinFree++;
  pstSerialCh->pstRxChannel->uinUsed--;

  // Now check to see if the upper threshold CtrlS has been issued whether the lower threshold
  // is now been reached.  If so then instruct the transmitted to send out a CtrlQ.
  if (pstSerialCh->pstRxChannel->boHysteresisActive &&
          (pstSerialCh->pstRxChannel->uinUsed <= pstSerialCh->pstRxChannel->uinCtrlQThreshold))
  {
    // We are below the lower threshold for used locations in the buffer.
    // So instruct the TX to send out a CtrlQ to restart characters.
    pstSerialCh->pstRxChannel->chCtrlSCtrlQ = CTRLQ;

    // Reset the flag to indicate we are no longer are the CtrlS mode.
    pstSerialCh->pstRxChannel->boHysteresisActive = False;

    // Make sure the the TX interrupts are enable so that this can be checked by the
    // transmitter and the ctrlQ is sent.
    ENABLE_TXE_INT(pstSerialCh->pstTxChannel);
  }
#ifdef __FREERTOS__
  __enable_irq();
#else
  HAL_NVIC_EnableIRQ(pstSerialCh->uinIRQn);
#endif

  // !!!!!! END OF CRITICAL SECTION !!!!!!

  return chTempChar;

} // end fchGetChar

/*
 * uin8_t fuinPutChar(stSERIAL_CHANNELTypeDef* pstSerialCh, enum BOOL boBlocking)
 * ------------------------------------------------------------------------------
 *
 * This function is called to put a character into the serial channel interrupt buffer.
 * It has two versions -- a blocking version that waits until a character position is
 * available, and a non-blocking version that returns if the buffer is full.
 *
 * Returns: number of free characters in the TX buffer.
 *
 */

uint8_t fuinPutChar(stSERIAL_CHANNELTypeDef *pstSerialCh, char chCharacter, enum BOOL boBlocking)
{
  uint8_t uinFreeLoc;

  // Get the number of free locations in the TX buffer
  // DISABLE_TXE_INT(pstSerialCh->pstTxChannel);
  uinFreeLoc = pstSerialCh->pstTxChannel->uinFree;
#ifdef __FREERTOS__
  if (!boBlocking)
  {
    // Check here to see if the semaphore is non-zero or not
    if (osSemaphoreGetCount(pstSerialCh->pstTxChannel->pvdTxSemaId) == 0)
    {
      return 255;
    }
  }
  osSemaphoreAcquire(pstSerialCh->pstTxChannel->pvdTxSemaId, osWaitForever);


#else
  // not __FREERTOS__
  // If non-blocking check to see if there is a free location.
  if (!boBlocking && (uinFreeLoc == 0))
  {
    // No locations in the transmit buffer so return
    return uinFreeLoc;
  }

  // Blocking mode to enter here, or free locations in non-blocking mode
  // Loop until space is available or the user has requested transmission to
  // commence.
  //
  // Note that the user CtrlS can result in the code execution being stuck here
  // for indeterminate periods of time.
  while (pstSerialCh->pstTxChannel->uinFree == 0)
  {;
  }
#endif

  // Enter this point if there is at least one free location in the TX buffer

  // !!!!!!!! START OF CRITICAL SECTION !!!!!!!!!
#ifdef __FREERTOS__
  __disable_irq();
#else
  HAL_NVIC_DisableIRQ(pstSerialCh->uinIRQn);
#endif
  // Now put the character passed in into the TX buffer.
  //DISABLE_TXE_INT(pstSerialCh->pstTxChannel);
  pstSerialCh->pstTxChannel->puchSerialBuffer[pstSerialCh->pstTxChannel->uinHead] = chCharacter;
  pstSerialCh->pstTxChannel->uinHead = (pstSerialCh->pstTxChannel->uinHead + 1) %
                                                        (pstSerialCh->pstTxChannel->uinBufSize);
  pstSerialCh->pstTxChannel->uinUsed++;
  pstSerialCh->pstTxChannel->uinFree--;
  uinFreeLoc = pstSerialCh->pstTxChannel->uinFree;

  // Only enable the transmit interrupts if the user has not requested transmission to
  // stop
  if (!(pstSerialCh->pstTxChannel->boTxUserCtrlS))
  {
    ENABLE_TXE_INT(pstSerialCh->pstTxChannel);
  }
#ifdef __FREERTOS__
  __enable_irq();
#else
  HAL_NVIC_EnableIRQ(pstSerialCh->uinIRQn);
#endif
  // !!!!!!!! END OF CRTICIAL SECTION !!!!!!!!
  return uinFreeLoc;

}

/*
 * uint16_t fuinGetAllChars(stSERIAL_CHANNELTypeDef* pstSerialCh, char* pchString)
 * -------------------------------------------------------------------------------
 *
 * This function is called to get all of the characters in the Rx serial buffer.
 * If there are multiple characters in the buffer then all of the characters are transferred
 * to the buffer array passed in, SUBJECT TO THE NUMBER OF CHARACTERS NOT BEING GREATER THAN
 * ONE LESS THAN THE BUFFER SIZE.
 *
 * The array of characters passed back has a \0 character placed on the end so that the
 * array is interrupted as a 'C' string variable.
 *
 * The routine is non-blocking -- it returns immediately with all of the current characters
 * in the buffer. If there are no characters then it still returns with no characters.
 *
 * Returns:  the length of the string.
 */

uint16_t fuinGetAllChars(stSERIAL_CHANNELTypeDef *pstSerialCh, char *pchString, uint16_t uinMaxStringSize)
{

  uint16_t uinStringIdx = 0;

  while (uinStringIdx < uinMaxStringSize)
  {
    if ((pchString[uinStringIdx] = fchGetChar(pstSerialCh, NON_BLOCKING)) == 255)
    {
      pchString[uinStringIdx] = NULL_CHAR;
      return uinStringIdx;
    }
    uinStringIdx++;
  }

  pchString[uinStringIdx] = NULL_CHAR;

  return uinStringIdx;

} // end fuinGetAllChars

/*
 * uint16_t fuinReceivedChars(stSERIAL_CHANNELTypeDef* pstSerialCh)
 * ----------------------------------------------------------------
 *
 * This routine returns the number of used spaces in the receive buffer --
 * in other words the number of received characters in the buffer.
 *
 * Returns: number of characters in receive buffer
 *
 */

uint16_t fuinReceivedChars(stSERIAL_CHANNELTypeDef *pstSerialCh)
{
  uint16_t uinNumRxChars;

  uinNumRxChars = pstSerialCh->pstRxChannel->uinUsed;

  return uinNumRxChars;
} // end fuinReceivedChars

/*
 * enum BOOL fboPrintString(stSERIAL_CHANNELTypeDef* pstSerialCh, char *pchString, enum BOOL boBlocking )
 * ------------------------------------------------------------------------------------------------------
 *
 * This routine outputs a \0 terminated string to the interrupt serial channel.
 *
 * NOTE: This routine implicitly assumes that the pchString pointer is pointing to a
 * proper 'C' string -- i.e. a \0 (NULL_CHAR) terminated ascii string.
 *
 * The routine attempts to send the whole string to the transmit buffer. If there
 * is not enough room in the buffer then the routine will wait until there is if the BLOCKING
 * attribute is set, else it will simply return a False if the string cannot be written.
 *
 * Returns: True if string send successful, else returns a False.
 */

enum BOOL fboPrintString(stSERIAL_CHANNELTypeDef *pstSerialCh, char *pchString, enum BOOL boBlocking)
{
  uint16_t uinStringIdx = 0;

  if (boBlocking == NON_BLOCKING)
  {
    if (strlen(pchString) <= pstSerialCh->pstTxChannel->uinFree)
    {
      while (pchString[uinStringIdx] != NULL_CHAR)
      {
        fuinPutChar(pstSerialCh, pchString[uinStringIdx], NON_BLOCKING);
        uinStringIdx++;
      }
      return True;
    }
    else
    {
      return False;
    }
  }
  else // BLOCKING
  {
    while (pchString[uinStringIdx] != NULL_CHAR)
    {
      fuinPutChar(pstSerialCh, pchString[uinStringIdx], BLOCKING);
      uinStringIdx++;
    }
    return True;
  }
}
