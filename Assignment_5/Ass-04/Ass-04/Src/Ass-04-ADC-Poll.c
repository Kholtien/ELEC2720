/*        $Id: Ass-04-ADC-Poll.c 1848 2019-05-12 18:14:49Z Peter $
 *  $Revision: 1848 $
 *      $Date: 2019-05-13 04:14:49 +1000 (Mon, 13 May 2019) $
 *    $Author: Peter $
 */

#include "Ass-04.h"

/*
 * The ADC has been set up to sequence both analog inputs and generate
 * an end of conversion after each analog input is converted.
 */

void
ADC_Poll (void)
{
  HAL_StatusTypeDef status;
  uint16_t ai[2];
  uint32_t i, j;

  printf ("ADC_Poll():\n");

  while (1)
  {
    // printf("-> Start conversion...\n");
    if ((status = HAL_ADC_Start (&hadc1)) != HAL_OK)
    {
      printf ("-> ERROR: HAL_ADC_Start() call failed (status = %d)\n", status);
      return;
    }

    // Loop through both channels to get result
    for (j = 0; j < 2; j++)
    {
      // printf("-> Wait for conversion to finish...\n");
      if ((status = HAL_ADC_PollForConversion (&hadc1, 10)) != HAL_OK)
      {
        printf (
            "-> ERROR: HAL_ADC_PollForConversion() call failed (status = %d)\n",
            status);
        return;
      }

      // printf("-> Get the value...\n");
      ai[j] = (uint16_t)HAL_ADC_GetValue (&hadc1);

    }

    // Loop through both channels to print result
    for (j = 0; j < 2; j++)
    {
      // Print the result as decimal and bar graph
      printf ("Got %4d: |", ai[j]);
      for (i = 0; i < 25; i++)
      {
        if (i <= ai[j] * 25 / 4096)
        {
          printf ("*");
        }
        else
        {
          printf (" ");
        }
      }
      printf ("| ");
    }
    printf ("\r");

    // Loop delay
    HAL_Delay (100);
  }
}
