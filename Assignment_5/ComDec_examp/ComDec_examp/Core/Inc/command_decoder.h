/* Revision Information
 * ====================
 *
 * $Id: command_decoder.h 5013 2021-05-23 23:02:53Z rbetz $
 * $Author: rbetz $
 * $Date: 2021-05-24 09:02:53 +1000 (Mon, 24 May 2021) $
 * $Revision: 5013 $
 */



/************************************************************************/
/*																																			*/
/*																																			*/
/*																																			*/
/*                      COMMAND DECODER HEADER FILE											*/
/*																																			*/
/*				  											 by																		*/
/*																																			*/
/*		             						 Robert Betz			  											*/
/*	     				Department of Electrical and Computer Engineering				*/
/*			 									University of Newcastle												*/
/*																																			*/
/*			    						  (Copyright 1992, 2021)												*/
/*																																			*/
/*																																			*/
/*																																			*/
/************************************************************************/


#ifndef COMMAND_DECODER_H_
#define COMMAND_DECODER_H_

#include "cmsis_os.h"

/* These definitions below define the way that the decode tables will work 
*/

typedef enum {ASCII_DECODING = 0, BINARY_DECODING = 1} decode_mode;

// Character to define the end of an ascii decoding table
#define ASC_TABLE_END 0xff

// Useful macro for setting up ranges in decoding tables
#define CHAR_RANGE(c1,c2) (c1 | 0x80), c2

#if !defined TRUE && !defined FALSE
#define TRUE 1
#define FALSE 0
#endif


#define CR_CHAR 0x0d
#define LF_CHAR 0x0a
#define CTRL_Q  0x11
#define CTRL_S  0x13
#define BS_CHAR 0x08
#define DEL_CHAR 0x7F

// Macro to delete a terminal character
#define SCRN_DEL_CHAR "\b \b"


// Define the NULL_PTR
#define NULL_PTR NULL

// Define the terminating pointer for task name tables
#define TERMINATING_PTR 0xffffffff

#define BOOL unsigned char



/************************************************************************/
/*																																			*/
/*		   COMMAND DECODER STRUCTURES				                              */
/*									                                                    */
/************************************************************************/


/*------ ENVELOPE STRUCTURE ------*/
/*
 * REB: ADDED IN MAY 2021 TO SUPPORT FreeRTOS VERSION OF DECODER.
 *
 * This structure can be customised by the user. The queues in FreeRTOS
 * can be of any type, but in general each item in a queue is a
 * structure of some type. This type will always be called an envelope
 * structure.
 *
 * pvdReceiverQueueId : pointer to the queue of the recipient thread.
 * pvdSendTaskQueueId : pointer to the queue for the sending task.
 * suinMessSize : size of the message being sent.
 * pvdMess : a point to a void type. This point is usually pointing to
 *           a linear section of memory with data it it of a size
 *           specified by the suinMessSize variable.
 *
 * uchDynamicMemFlag : if TRUE then the memory pointed to be pvdMess
 *            has been dynamically allocated by the called task, and
 *            therefore has to be deallocated in the decoder.
 *
 */


typedef struct stMessEnvelope
{
  osMessageQueueId_t pvdRecipientQueueId;   // The queue handle of the recipient thread
  osMessageQueueId_t pvdSendTaskQueueId;    // The queue handle of the sender thread
  uint32_t suinMessSize;                    // The size of the message in bytes
  uint8_t *puchMess;                        // a pointer to the actual message buffer
  uint8_t uchDynamicMemFlag;                // if TRUE then dynamic allocation is used.
} tstMessEnvelope;


/*------ CHARACTER DECODER INPUT STRUCTURE ------*/
/* This structure is used by the character decode routine in the COMDEC
module. Refer to the UNOS documentation for more details.
*/


typedef struct decode_struc
{
  decode_mode char_dec_mode;
  unsigned char data_byte;
  unsigned char *valid_data_table_ptr;
  void ( **vector_table_ptr ) ( struct decode_struc * );
  unsigned char *bit_table_ptr;
  void *info_struc_ptr;
} dec_input_struc;




/*------ MESSAGE ADDRESS DECODE STRUCTURE ------*/
/* Top level structure used in the command decoder.
 * This structure is used by the receiving task to determine if the
 * sending task is a valid sender to this receiving task.
 *
 * The valid_addr_tab_ptr points to a table of pointers
 * that contain the address of the queues of tasks that
 * can send information to the current receiving task.
 *
 * NOTE: this concept works because each task that is
 * created in the system has its own unique input queue
 * with a name of the form
 *
 *   <task name>QHandle
 *
 * where <task name> means the actual task name of
 * the task.
 *
 * The addt_vector_ptr points to the valid decoder
 * structure for a valid task queue handle.
 *
 * If a valid taks queue handle is not found then a
 * default error dec_input_struc will be selected and
 * the error will be handled in the routines associated
 * with this.
*/

typedef struct
{
  osMessageQueueId_t pvdTaskQueueId;
  osMessageQueueId_t pvdRtnAddrQueueId;
  osMessageQueueId_t *valid_addr_tab_ptr;
  dec_input_struc **addr_vector_ptr;
  void *info_struc_ptr;
  osMemoryPoolId_t pvdMemoryPoolId;
} addr_struc;







// Function prototypes - these are the only routines thaare allowed as an entry
// points to the command decoder.
extern osStatus_t decode_mbx_mess ( addr_struc *addr_struc_ptr, unsigned long
                                time_limit );

extern void char_decode ( dec_input_struc *input_ptr );
#endif

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END OF FILE >>>>>>>>>>>>>>>>>>>>>>>>>*/
