/*        $Id: Ass-01-Operators.c 4642 2021-02-12 12:57:13Z rbetz $
 *  $Revision: 4642 $
 *      $Date: 2021-02-12 23:57:13 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int finOperators_main (int argc, char *argv[])
{
  uint8_t uchA1 = 20;
  uint8_t uchA2 = 15;
  uint8_t uchA3;
  uint16_t usinB1 = 20;
  uint16_t usinB2 = 15;
  uint16_t usinB3;
  float sfF1 = 20.0;
  float sfF2 = 15.0;
  float sfF3;

  printf ("a) Arithmetic operations:\n");
  uchA3 = uchA1 + uchA2;
  usinB3 = usinB1 + usinB2;
  sfF3 = sfF1 + sfF2;
  printf ("   uchA1+uchA2 = %d, usinB1+usinB2 = %d, sfF1+sfF2 = %f\n", uchA3, usinB3, sfF3);
  uchA3 = uchA1 - uchA2;
  usinB3 = usinB1 - usinB2;
  sfF3 = sfF1 - sfF2;
  printf ("   uchA1-uchA2 = %d, usinB1-usinB2 = %d, sfF1-sfF2 = %f\n", uchA3, usinB3, sfF3);
  uchA3 = uchA1 * uchA2;
  usinB3 = usinB1 * usinB2;
  sfF3 = sfF1 * sfF2;
  printf ("   uchA1*uchA2 = %d, usinB1*usinB2 = %d, sfF1*sfF2 = %f\n", uchA3, usinB3, sfF3);
  uchA3 = uchA1 / uchA2;
  usinB3 = usinB1 / usinB2;
  sfF3 = sfF1 / sfF2;
  printf ("   uchA1/uchA2 = %d, usinB1/usinB2 = %d, sfF1/sfF2 = %f\n", uchA3, usinB3, sfF3);
  uchA3 = uchA1 % uchA2;
  usinB3 = usinB1 % usinB2;
  printf ("   uchA1%%uchA2 = %d, usinB1%%usinB2 = %d\n", uchA3, usinB3);

  printf ("b) Effect of implicit and explicit type casting:\n");
  sfF3 = uchA1 / uchA2;
  printf ("   uchA1/uchA2 = %f (implicit)\n", sfF3);
  sfF3 = (float)uchA1 / (float)uchA2;
  printf ("   uchA1/uchA2 = %f (explicit)\n", sfF3);

  printf ("c) Comparison operations:\n");
  printf ("   uchA1==uchA2 = %d, uchA1!=uchA2 = %d\n", uchA1 == uchA2, uchA1 != uchA2);
  printf ("   uchA1<uchA2  = %d, uchA1>uchA2  = %d\n", uchA1 < uchA2, uchA1 > uchA2);
  printf ("   uchA1<=uchA2 = %d, uchA1>=uchA2 = %d\n", uchA1 <= uchA2, uchA1 >= uchA2);

  return 0;
}
