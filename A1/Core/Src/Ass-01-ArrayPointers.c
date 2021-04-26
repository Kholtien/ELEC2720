/*        $Id: Ass-01-ArrayPointers.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int finArrayPointers_main (int argc, char *argv[])
{
  int16_t sinI, sinJ, sinK; // Declare three int16_t variables
  int16_t *a3psinA[3]; // Declare an array of three pointers to int16_t

  int8_t uchX;
  int16_t sinY;
  float sfZ;
  void *a3pvdB[3];    // Declare an array of three void pointers;

  printf ("a) Indexing array elements:\n");
  sinI = 12;
  sinJ = 34;
  sinK = 56;
  a3psinA[0] = &sinI;
  a3psinA[1] = &sinJ;
  a3psinA[2] = &sinK;
  printf ("   a3psinA[0] = %p,  a3psinA[1] = %p, a3psinA[2] = %p\n",
		  a3psinA[0], a3psinA[1], a3psinA[2]);
  printf ("   *a3psinA[0] = %d, *a3psinA[1] = %d, *a3psinA[2] = %d\n",
          *a3psinA[0], *a3psinA[1], *a3psinA[2]);

  printf ("b) Pointers to different data types:\n");
  uchX = 11;
  sinY = 22;
  sfZ = 3.3;
  a3pvdB[0] = &uchX;
  a3pvdB[1] = &sinY;
  a3pvdB[2] = &sfZ;
  printf ("   a3pvdB[0] = %p, a3pvdB[1] = %p, a3pvdB[2] = %p\n",
		  a3pvdB[0], a3pvdB[1], a3pvdB[2]);
  printf (
      "   *(int8_t *)a3pvdB[0] = %d, "
      "*(int16_t *)a3pvdB[1] = %d, *(float *)a3pvdB[2] = %f\n",
      *(int8_t *)a3pvdB[0], *(int16_t *)a3pvdB[1], *(float *)a3pvdB[2]);
  printf (
      "   &(*(int8_t *)a3pvdB[0]) = %p, "
      "&(*(int16_t *)a3pvdB[1]) = %p, &(*(float *)a3pvdB[2]) = %p\n",
      &(*(int8_t *)a3pvdB[0]),
      &(*(int16_t *) a3pvdB[1]), &(*(float *)a3pvdB[2]));

  return 0;
}
