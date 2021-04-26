/*        $Id: Ass-01.c 4642 2021-02-12 12:57:13Z rbetz $
 *  $Revision: 4642 $
 *      $Date: 2021-02-12 23:57:13 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

// Calls to all of the assignment exercise components

#include "Ass-01.h"

int finLabExercise_Main (void)
{
  int n = -1;
  // Welcome
  // printf("\014");
  printf ("\n");
  printf ("%s\n", ASS_STRING);
  printf ("%s\n", VER_STRING);

  // Call exercise functions

  printf ("\n%d. First C program to print a message to the console:\n", ++n);
#if defined(DO_HELLO_WORLD) || defined(DO_ALL)
  finHelloWorld_main (0, 0);
#else
  printf ("Skipped.\n");
#endif

  printf ("\n%d. Reading parameters from the command line:\n", ++n);
#if defined(DO_PASSING_PARAMETERS) || defined(DO_ALL)
  {
    char *argv[] =
      { "./PassingParameters", "one", "two", "three" };
    int argc = sizeof(argv) / sizeof(char *);
    finPassingParameters_main (argc, argv);
  }
#else
  printf ("Skipped.\n");
#endif

  printf ("\n%d. Output formatted strings:\n", ++n);
#if defined(DO_FORMATTED_OUTPUT) || defined(DO_ALL)
  finFormattedOutput_main (0, 0);
#else
  printf ("Skipped.\n");
#endif

  printf ("\n%d. Preprocessor directives:\n", ++n);
#if defined(DO_PREPROCESSOR) || defined(DO_ALL)
  finPreprocessor_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Data types:\n", ++n);
#if defined(DO_DATA_TYPES) || defined(DO_ALL)
  finDataTypes_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Data structures:\n", ++n);
#if defined(DO_DATA_STRUCT) || defined(DO_ALL)
  finDataStruct_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. ASCII Characters:\n", ++n);
#if defined(DO_ASCII) || defined(DO_ALL)
  finASCII_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Operators:\n", ++n);
#if defined(DO_OPERATORS) || defined(DO_ALL)
  finOperators_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Pointers:\n", ++n);
#if defined(DO_POINTERS) || defined(DO_ALL)
  finPointers_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Arrays:\n", ++n);
#if defined(DO_ARRAYS) || defined(DO_ALL)
  finArrays_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Array of pointers:\n", ++n);
#if defined(DO_ARRAY_POINTERS) || defined(DO_ALL)
  finArrayPointers_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Flow control:\n", ++n);
#if defined(DO_FLOW_CONTROL) || defined(DO_ALL)
  finFlowControl_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Memory Allocation:\n", ++n);
#if defined(DO_MEMORY_ALLOCATION) || defined(DO_ALL)
  finMemoryAllocation_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Memory Management:\n", ++n);
#if defined(DO_MEMORY_MANAGEMENT) || defined(DO_ALL)
  finMemoryManagement_main (0, 0);
#else
  printf("Skipped.\n");
#endif

  printf ("\n%d. Functions and Parameter Passing:\n", ++n);
#if defined(DO_FUNCTIONS) || defined(DO_ALL)
  finFunctions_main (0, 0);
#else
  printf("Skipped.\n");
#endif

// Example of crash due to NULL pointer dereference as covered in Week 3 lecture

/*
  {
    char *c;
    c = 0;
    *c = 0;
  }
*/

  // Always return pass for now
  printf ("\nDone.\n");
  return 0;

}
