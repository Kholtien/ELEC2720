/*
 * colton.c
 *
 *  Created on: 19 Apr 2021
 *      Author: c3267533
 */

//#include "usart_io_retarget.h"// Added
//#include <stdio.h>// Added
//
//
////UART_HandleTypeDef huart2;
//
//
//
//void assMain(void)
//{
//	while(1){
//	}
//}




//
//typedef struct
//{
//	stRX_SERIAL_CHANNELTypeDef * pstRxChannel ; // Pointer to the RX data structure
//	stTX_SERIAL_CHANNELTypeDef * pstTxChannel ; // Pointer to the TX data structure
//	uint8_t uinIRQn ; 							// The interrupt number for the UART
//	uint32_t ulSerialErrorFlags ; 				// To store the error flags
//	uint32_t ulSerialErrorCodes ; 				// TO store error codes serial channel
//} stSERIAL_CHANNEL;
//
//typedef struct
//{
//	UART_HandleTypeDef * pstUartHALHandle ; 	// Pointer to the HAL USART Structure .
//	uint16_t uinHead ; 							// The buffer head index .
//	uint16_t uinTail ; 							// The buffer tail index .
//	uint16_t uinFree ; 							// Number of free buffer locs .
//	uint16_t uinUsed ; 							// Number of unused buffer locs .
//	uint16_t uinBufSize ; 						// The size of the buffer .
//	int8_t chCtrlSCtrlQ ; 						// Set to ctrl S or Q if this should be
//												// sent immediately out of the Tx , else a 0
//												// This CtrlS /Q is related to the Rx buffer state .
//	enum BOOL boHysteresisActive ; 				// True if waiting for CtrlQ threshold .
//	uint16_t uinCtrlSThreshold ; 				// The number of free locations where
//												// a Ctrl S has been sent .
//	uint16_t uinCtrlQThreshold ; 				// The number of free locations when
//												// a Ctrl Q should be sent .
//	enum BOOL boOverflowErr ; 					// Overflow error flag .
//	enum BOOL boParityErr ; 					// Parity error flag .
//	int8_t * pchSerialBuffer ; 						// Pointer to the serial buffer
//} stRX_SERIAL_CHANNEL;
//
//
//typedef struct
//{
//	UART_HandleTypeDef * pstUartHALHandle ; 	// Pointer to the HAL USART Structure .
//	uint16_t uinHead ; 							// The buffer head index .
//	uint16_t uinTail ; 							// The buffer tail index .
//	uint16_t uinFree ; 							// Number of free buffer locs .
//	uint16_t uinUsed ; 							// Number of unused buffer locs .
//	uint16_t uinBufSize ; 						// The size of the buffer .
//	enum BOOL boTxUserCtrlS ; 					// True if the user requests stop of Tx.
//	int8_t * pchSerialBuffer ; 					// Pointer to the serial buffer
//} stTX_SERIAL_CHANNEL;
