/*        $Id: Ass-04.c 1836 2019-05-06 18:01:25Z Peter $
 *  $Revision: 1836 $
 *      $Date: 2019-05-07 04:01:25 +1000 (Tue, 07 May 2019) $
 *    $Author: Peter $
 */

#include "Ass-04.h"

int
ADC_main (void)
{

  // Welcome
  // printf("\014");
  printf ("\n");
  printf ("%s\n", ASS_STRING);
  printf ("%s\n", VER_STRING);
#ifdef STM32F407xx
  printf ("Running on STM32 platform\n");
#else
  printf ("Running on WINNT platform\n");
#endif

#ifdef DO_QUESTION_1
  // Call ADC with polling function
  printf("-> Question 1\n");
#ifdef STM32F407xx
ADC_Poll();
#else
  printf ("-> No code implemented for WINNT at the moment.\n");
#endif
#endif

#ifdef DO_QUESTION_2
  // Call ADC with interrupt function
  printf("-> Question 2\n");
#ifdef STM32F407xx
ADC_Interrupt();
#else
  printf ("-> No code implemented for WINNT at the moment.\n");
#endif
#endif

#ifdef DO_QUESTION_3
  // Call ADC with DMA function
  printf("-> Question 3\n");
#ifdef STM32F407xx
ADC_DMA();
#else
  printf ("-> No code implemented for WINNT at the moment.\n");
#endif
#endif

  // Sign off message
  printf ("Done.\n");
  return 0;

}
