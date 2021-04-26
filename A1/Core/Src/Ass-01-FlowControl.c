/*        $Id: Ass-01-FlowControl.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int finFlowControl_main (int argc, char *argv[])
{
  int16_t sinA = 1;
  int16_t sinI;

  printf ("a) Use of the if statement:\n");
  printf ("   sinA = %d\n", sinA);
  printf ("   ");
  if (sinA == 1)
  {
    printf ("sinA is 1\n");
  }
  else if (sinA == 2)
  {
    printf ("sinA is 2\n");
  }
  else
  {
    printf ("sinA not 1 or 2\n");
  }

  printf ("b) Use of the switch statement:\n");
  printf ("   sinA = %d\n", sinA);
  printf ("   ");
  switch (sinA)
    {
    case 1:
      printf ("sinA is 1\n");
      break;
    case 2:
      printf ("sinA is 2\n");
      break;
    default:
      printf ("sinA not 1 or 2\n");
    }

  printf ("c) Use of for loop:\n");
  for (sinI = 0; sinI < 4; sinI++)
  {
    printf ("   sinI = %d\n", sinI);
  }
  printf ("   sinI = %d (after loop)\n", sinI);

  printf ("d) Use of while loop:\n");
  sinI = 0;
  while (sinI < 4)
  {
    printf ("   sinI = %d\n", sinI);
    sinI++;
  }
  printf ("   sinI = %d (after loop)\n", sinI);

  printf ("e) Use of do while loop:\n");
  sinI = 0;
  do
  {
    printf ("   sinI = %d\n", sinI);
    sinI++;
  }
  while (sinI < 4);
  printf ("   sinI = %d (after loop)\n", sinI);

  return 0;
}
