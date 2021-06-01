/*
 * Serial_Driver.h
 *
 *  Created: October 28, 2020
 *  Created by: Bob Betz
 *  Last modified by: $Author: rbetz $
 *  Modified on: $Date: 2021-05-26 22:07:07 +1000 (Wed, 26 May 2021) $ UTC
 *  Version: $Revision: 5025 $
 */
#include "main.h"
#include <stdlib.h>
#include <string.h>

#ifndef INC_SERIAL_DRIVER_H_
#define INC_SERIAL_DRIVER_H_

/*
 * FreeRTOS Selection
 *
 * The following define should be uncommented if one wishes to use the
 * serial buffers with the FreeRTOS system.
 */
#define __FREERTOS__
#ifdef __FREERTOS__
// Set up macros for the memory allocation.
// FreeRTOS cannot use the normal malloc and associated
// C library memory allocation routines.
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#define MEMORY_ALLOC(NUM_BYTES) (pvPortMalloc(NUM_BYTES))
void *pvPortMalloc(size_t);
#else
#define MEMORY_ALLOC(NUM_BYTES) (calloc(NUM_BYTES, sizeof(int8_t)))
#endif


// Set up the buffer sizes
#define TX_BUFFER_SIZE 40
#define RX_BUFFER_SIZE 40
// Set up the level to generate Ctrl S and Ctrl Q for Rx buffer
#define RX_CTRLS_LEVEL RX_BUFFER_SIZE * 0.3
#define RX_CTRLQ_LEVEL RX_BUFFER_SIZE * 0.0


/*
 * Define specific error codes for this serial handler
 */
#define INTERRUPT_GEN_ERROR ((0x01U))     // The interrupt generated an error
#define RX_BUFFER_FULL ((0x01U) << 1)     // Rx buffer is full upon receive interrupt

enum BOOL {False, True};

// USART interrupt enable and disable macros
// enable the RX interrupt
#define ENABLE_RXNE_INT(RX_CHANNEL) SET_BIT((RX_CHANNEL->pstUartHALHandle->Instance->CR1), USART_CR1_RXNEIE)
// enable the TX interrupt
#define ENABLE_TXE_INT(TX_CHANNEL) SET_BIT((TX_CHANNEL->pstUartHALHandle->Instance->CR1), USART_CR1_TXEIE)
// disable the RX interrupt
#define DISABLE_RXNE_INT(RX_CHANNEL) CLEAR_BIT((RX_CHANNEL->pstUartHALHandle->Instance->CR1), USART_CR1_RXNEIE)
// disable the TX interrupt
#define DISABLE_TXE_INT(TX_CHANNEL) CLEAR_BIT((TX_CHANNEL->pstUartHALHandle->Instance->CR1), USART_CR1_TXEIE)

#define CTRLS 0x13        // The Control S ascii character -- STOP transmission
#define CTRLQ 0x11        // The Control Q ascii character -- START transmission
#define NULL_CHAR '\0'    // Used to indicate the end of strings.

#define BLOCKING True       // Blocking call to serial routine
#define NON_BLOCKING False      // Non-blocking call to serial routine

// Set up the bits used to check for serial transmission errors
// Actually defined in the auto generated HAL code
//#define UART_OVERRUN_ERR ((0x01U) << 3)   // The bit that indicates an overrun error.
//#define UART_NOISE_ERR ((0x01U) << 2)     // Noise glitch detected error.
//#define UART_FRAMING_ERR ((0x01U) << 1)   // Framing error.
//#define UART_PARITY_ERR ((0x01U) << 0)    // Parity error


/*
 * Define the data structures used in Serial Channels
 */

typedef struct
{
  UART_HandleTypeDef *pstUartHALHandle;   // Pointer to the HAL USART Structure.
  uint16_t uinHead;             // The buffer head index.
  uint16_t uinTail;             // The buffer tail index.
  uint16_t uinFree;             // Number of free buffer locs.
  uint16_t uinUsed;             // Number of unused buffer locs.
  uint16_t uinBufSize;          // The size of the buffer.
  int8_t chCtrlSCtrlQ;          // Set to ctrl S or Q if this should be
                                // sent immediately out of the Tx, else a 0
                                // This CtrlS/Q is related to the Rx buffer state.
  enum BOOL boHysteresisActive; // True if waiting for CtrlQ threshold.
  uint16_t uinCtrlSThreshold;   // The number of free locations where
                                // a Ctrl S has been sent.
  uint16_t uinCtrlQThreshold;   // The number of free locations when
                                // a Ctrl Q should be sent.
  enum BOOL boOverflowErr;      // Overflow error flag.
  enum BOOL boParityErr;        // Parity error flag.
  uint8_t *puchSerialBuffer;    // Pointer to the serial buffer
#ifdef __FREERTOS__
  osSemaphoreId_t pvdRxSemaId;   // Pointer to the receive semaphore
#endif
} stRX_SERIAL_CHANNELTypeDef;

typedef struct
{
  UART_HandleTypeDef *pstUartHALHandle;   // Pointer to the HAL USART Structure.
  uint16_t uinHead;             // The buffer head index.
  uint16_t uinTail;             // The buffer tail index.
  uint16_t uinFree;             // Number of free buffer locs.
  uint16_t uinUsed;             // Number of unused buffer locs.
  uint16_t uinBufSize;          // The size of the buffer.
  enum BOOL boTxUserCtrlS;      // True if the user requests stop of Tx.
  uint8_t *puchSerialBuffer;    // Pointer to the serial buffer
#ifdef __FREERTOS__
  osSemaphoreId_t pvdTxSemaId;   // Pointer to the transmit semaphore.
#endif
} stTX_SERIAL_CHANNELTypeDef;


typedef struct
{
  stRX_SERIAL_CHANNELTypeDef *pstRxChannel;   // Pointer to the RX data structure
  stTX_SERIAL_CHANNELTypeDef *pstTxChannel;   // Pointer to the TX data structure
  uint8_t uinIRQn;                            // The interrupt number for the UART
  uint32_t ulSerialErrorFlags;                // To store the error flags
  uint32_t ulSerialErrorCodes;                // TO store error codes serial channel
} stSERIAL_CHANNELTypeDef;


extern stSERIAL_CHANNELTypeDef* pstSerialChannel2;

/*
 * Function Prototypes
 */
extern stSERIAL_CHANNELTypeDef* pstCreateSerialChannel(
                                                        UART_HandleTypeDef *pstHuart,
                                                        uint16_t uinRxBufSize,
                                                        uint16_t uinTxBufSize
#ifdef __FREERTOS__
                                                        ,
                                                        osSemaphoreId_t pvdRxSemaId,  // The transmit buffer semaphore
                                                        osSemaphoreId_t pvdTxSemaId   // The receive buffer semaphore

#endif
);


extern void fvdSerialChannelISR(stSERIAL_CHANNELTypeDef* pstSerialCh);

extern void fvdEnableSerialChInterrupts(stSERIAL_CHANNELTypeDef* pstSerialCh);

extern void fvdRxInterruptHandler(stSERIAL_CHANNELTypeDef* pstSerialCh);

extern void fvdTxInterruptHandler(stSERIAL_CHANNELTypeDef* pstSerialCh);

extern char fchGetChar(stSERIAL_CHANNELTypeDef *pstSerialCh, enum BOOL boBlocking);

extern uint8_t fuinPutChar(stSERIAL_CHANNELTypeDef *pstSerialCh, char chCharacter, enum BOOL boBlocking);

extern uint16_t fuinGetAllChars(stSERIAL_CHANNELTypeDef *pstSerialCh, char *pchString, uint16_t uinMaxStringSize);

extern uint16_t fuinReceivedChars(stSERIAL_CHANNELTypeDef* pstSerialCh);

extern enum BOOL fboPrintString(stSERIAL_CHANNELTypeDef *pstSerialCh, char *pchString, enum BOOL boBlocking);

#endif /* INC_SERIAL_DRIVER_H_ */
