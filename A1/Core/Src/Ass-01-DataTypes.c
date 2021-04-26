/*        $Id: Ass-01-DataTypes.c 4642 2021-02-12 12:57:13Z rbetz $
 *  $Revision: 4642 $
 *      $Date: 2021-02-12 23:57:13 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int finDataTypes_main (int argc, char *argv[])
{

//	  struct myFirstStruct
//	  {
//	    char chC;
//	    int inI;
//	    float sfF;
//	  } stA = { 'a', 1, 2.34 };
  printf ("a) Standard C data type sizes:\n");
  printf ("   -> sizeof(char)        = %2d\n", sizeof(char));
  printf ("   -> sizeof(short)       = %2d\n", sizeof(short));
  printf ("   -> sizeof(int)         = %2d\n", sizeof(int));
  printf ("   -> sizeof(long)        = %2d\n", sizeof(long));
  printf ("   -> sizeof(long long)   = %2d\n", sizeof(long long));
  printf ("   -> sizeof(float)       = %2d\n", sizeof(float));
  printf ("   -> sizeof(double)      = %2d\n", sizeof(double));
  printf ("   -> sizeof(long double) = %2d\n", sizeof(long double));

  printf ("b) Standard integer data type sizes:\n");
  printf ("   -> sizeof(int8_t)      = %2d\n", sizeof(int8_t));
  printf ("   -> sizeof(int16_t)     = %2d\n", sizeof(int16_t));
  printf ("   -> sizeof(int32_t)     = %2d\n", sizeof(int32_t));
  printf ("   -> sizeof(int64_t)     = %2d\n", sizeof(int64_t));

  printf ("c) Pointer data sizes (note they are all the same):\n");
  printf ("   -> sizeof(char *)      = %2d\n", sizeof(char *));
  printf ("   -> sizeof(short *)     = %2d\n", sizeof(short *));
  printf ("   -> sizeof(int *)       = %2d\n", sizeof(int *));
  printf ("   -> sizeof(long *)      = %2d\n", sizeof(long *));
  printf ("   -> sizeof(long long *) = %2d\n", sizeof(long long *));


  printf ("d) Assignment stuff:\n");
  printf ("   -> sizeof(uint16_t)     = %2d\n", sizeof(uint16_t));
//  printf ("   -> sizeof(stA)     = %2d\n", sizeof(stA));

  return 0;
}
