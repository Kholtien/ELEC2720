/*        $Id: Ass-01-Arrays.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int finArrays_main (int argc, char *argv[])
{
  int16_t sinA[3];    // Declare an array of three int16_t

  printf ("a) Indexing array elements:\n");
  sinA[0] = 0;
  sinA[1] = 1;
  sinA[2] = 2;
  printf ("   sinA[0] = %d, sinA[1] = %d, sinA[2] = %d\n",
          sinA[0], sinA[1], sinA[2]);

  printf ("b) Array name as pointer:\n");
  printf ("   sinA = %p\n", sinA);
  printf ("   &(sinA[0]) = %p, &(sinA[1]) = %p, &(sinA[2]) = %p\n",
          &(sinA[0]), &(sinA[1]), &(sinA[2]));

  printf ("c) Pointer arithmetic:\n");
  printf ("   *(sinA+0) = %d, *(sinA+1) = %d, *(sinA+2) = %d\n",
          *(sinA + 0), *(sinA + 1), *(sinA + 2));
  printf ("   (sinA+0) = %p, (sinA+1) = %p, (sinA+2) = %p\n",
          (sinA + 0), (sinA + 1), (sinA + 2));

  return 0;
}
