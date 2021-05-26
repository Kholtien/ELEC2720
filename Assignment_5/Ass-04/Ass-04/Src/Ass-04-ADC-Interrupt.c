/*        $Id: Ass-04-ADC-Interrupt.c 1848 2019-05-12 18:14:49Z Peter $
 *  $Revision: 1848 $
 *      $Date: 2019-05-13 04:14:49 +1000 (Mon, 13 May 2019) $
 *    $Author: Peter $
 */

#include "Ass-04.h"

/*
 * The ADC has been set up to sequence both analog inputs.
 * The interrupt is at the end of the sequence after both conversions.
 * Without DMA, only the last value is read.
 */

volatile static uint16_t ai;
volatile static uint8_t got_ai = 0;

#ifdef DO_QUESTION_2
void
HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc)
{
  if (hadc == &hadc1)
  {
    ai = (uint16_t) HAL_ADC_GetValue (&hadc1);
    got_ai = 1;
  }
}
#endif

void
ADC_Interrupt (void)
{
  printf ("ADC_Interrupt():\n");

  HAL_StatusTypeDef status;
  uint32_t i;

  while (1)
  {
    // printf("-> Start conversion...\n");
    got_ai = 0;
    if ((status = HAL_ADC_Start_IT (&hadc1)) != HAL_OK)
    {
      printf ("-> ERROR: HAL_ADC_Start_IT() call failed (status = %d)\n",
              status);
      return;
    }

    // printf("-> Wait for conversion to finish...\n");
    while (got_ai == 0) // Critical section protection not included
    {
    }

    // Print the result as decimal and bar graph
    printf ("Got %4d: |", ai);
    for (i = 0; i < 60; i++)
    {
      if (i <= ai * 60 / 4096)
      {
        printf ("*");
      }
      else
      {
        printf (" ");
      }
    }
    printf ("|\r");

    // Loop delay
    HAL_Delay (100);
  }

}
