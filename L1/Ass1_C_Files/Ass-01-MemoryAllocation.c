/*        $Id: Ass-01-MemoryAllocation.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int finMemoryAllocation_main (int argc, char *argv[])
{
#define NUM_VALUES 100
  int32_t *pinA; // Pointer to memory to be allocated

  printf ("a) Allocate memory:\n");
  pinA = (int32_t *) malloc ((size_t) (NUM_VALUES * sizeof(int16_t)));
  if (pinA == NULL)
  {
    printf ("   ERROR: Could not allocate memory");
    return 1;
  }
  printf ("   pinA = %p\n", pinA);

  printf ("b) Free memory:\n");
  free (pinA);
  printf ("   pinA = %p\n", pinA);

  return 0;
}
