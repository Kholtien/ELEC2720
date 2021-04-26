/*        $Id: Ass-01-DataStructures.c 4642 2021-02-12 12:57:13Z rbetz $
 *  $Revision: 4642 $
 *      $Date: 2021-02-12 23:57:13 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int finDataStruct_main (int argc, char *argv[])
{
  struct myFirstStruct
  {
    char chC;
    int inI;
    float sfF;
  } stA = { 'a', 1, 2.34 };

  // Define a structure with bit fields of various widths.
  struct mySecondStruct
  {
    unsigned uinMem1 :3;
    unsigned uinMem2 :2;
    unsigned uinMem3 :2;
    unsigned uinMem4 :1;
  } stB;

  uint8_t *puchC;

  printf ("a) Elements in a (initialised at declaration):\n");
  printf ("   a.c = '%c'\n", stA.chC);
  printf ("   a.i = '%d'\n", stA.inI);
  printf ("   a.f = '%f'\n", stA.sfF);

  printf ("b) Elements in a (modified values):\n");
  stA.chC = 'b';
  stA.inI = 2;
  stA.sfF = 5.67;
  printf ("   stA.chC = '%c'\n", stA.chC);
  printf ("   stA.inI = '%d'\n", stA.inI);
  printf ("   stA.sfF = '%f'\n", stA.sfF);

  printf ("c) Elements in the bit field:\n");
  stB.uinMem1 = 1;
  stB.uinMem2 = 2;
  stB.uinMem3 = 2;
  stB.uinMem4 = 1;
  printf ("   stB.uinMem1 = %d\n", stB.uinMem1);
  printf ("   stB.uinMem2 = %d\n", stB.uinMem2);
  printf ("   stB.uinMem3 = %d\n", stB.uinMem3);
  printf ("   stB.uinMem4 = %d\n", stB.uinMem4);

  printf ("d) Overlaying bit field:\n");
  puchC = (uint8_t *) &stB;
  printf ("   puchC      = 0x%02x\n", *puchC);

  return 0;
}
