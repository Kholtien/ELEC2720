/*        $Id: Ass-01-ASCII.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>

// Uncomment to enable reading characters from the console
//
// #define READ_INPUT

int finASCII_main (int argc, char *argv[])
{
#ifdef READ_INPUT
  int inI;
  int inC;
#endif

  printf ("a) ASCII values for some characters:\n");
  printf ("   %3d => '%c'\n", 'A', 'A');
  printf ("   %3d => '%c'\n", 'a', 'a');
  printf ("   %3d => '%c'\n", '1', '1');
  printf ("   %3d => '%c'\n", '2', '2');
  printf ("b) Some escape sequences:\n");
  printf ("   %3d => '%c'\n", '\n', '\n');
  printf ("   %3d => '%c'\n", '\r', '\r');
  printf ("   %3d => '%c'\n", '\t', '\t');
  printf ("   %3d => '%c'\n", '\007', '\007');
  printf ("c) Read five characters:\n");

#ifdef READ_INPUT
  for (inI=0;inI<5;inI++)
  {
    // fflush(stdout);
    while ((inC=getchar()) < 0);
    // printf("The counter = %d\n", inI);
    printf ("   Read %3d => '%c'\n", inC, inC);
  }
#else
  printf ("   -> Not enabled.\n");
#endif

  return 0;
}
