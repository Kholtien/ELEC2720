/*        $Id: Ass-01.h 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */
// NB: Based on previous labs developed by Peter Stepien
// Common include file for all modules

#ifndef ASS_01_H_
#define ASS_01_H_

// Assignment and version strings
#define ASS_STRING "ELEC2720-2021s1 Assignment 1"
#define VER_STRING "Ver: 01 $Rev: 4655 $"

// Exercises to be completed
// Can select all exercises or specific exercises

// #define DO_ALL

//#define DO_HELLO_WORLD
// #define DO_PASSING_PARAMETERS
// #define DO_FORMATTED_OUTPUT
// #define DO_PREPROCESSOR
// #define DO_DATA_TYPES
// #define DO_DATA_STRUCT
// #define DO_ASCII
// #define DO_OPERATORS
// #define DO_POINTERS
// #define DO_ARRAYS
// #define DO_ARRAY_POINTERS
// #define DO_FLOW_CONTROL
// #define DO_STREAMS
// #define DO_MEMORY_ALLOCATION
// #define DO_MEMORY_MANAGEMENT
 #define DO_FUNCTIONS

// Standard includes

#include "stm32f4xx_hal.h"
#include <stdio.h>

// Laboratory Exercise main function prototype and extern declaration

extern int finLabExercise_Main (void);

// Function Prototypes and External Declarations for each of the lab exercises

extern int finHelloWorld_main (int argc, char *argv[]);
extern int finPassingParameters_main (int argc, char *argv[]);
extern int finFormattedOutput_main (int argc, char *argv[]);
extern int finPreprocessor_main (int argc, char *argv[]);
extern int finDataTypes_main (int argc, char *argv[]);
extern int finDataStruct_main (int argc, char *argv[]);
extern int finASCII_main (int argc, char *argv[]);
extern int finOperators_main (int argc, char *argv[]);
extern int finPointers_main (int argc, char *argv[]);
extern int finArrays_main (int argc, char *argv[]);
extern int finArrayPointers_main (int argc, char *argv[]);
extern int finFlowControl_main (int argc, char *argv[]);
extern int finMemoryAllocation_main (int argc, char *argv[]);
extern int finMemoryManagement_main (int argc, char *argv[]);
extern int finFunctions_main (int argc, char *argv[]);

#endif /* ASS_01_H_ */
