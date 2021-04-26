/*        $Id: Ass-01-FormattedOutput.c 4642 2021-02-12 12:57:13Z rbetz $
 *  $Revision: 4642 $
 *      $Date: 2021-02-12 23:57:13 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
int finFormattedOutput_main (int argc, char *argv[])
{
  // Declare some variables and initialise them
  char a0chS[] = { "This is a string" };
  char chC = '0';
  int inI = 1234;
  float sfF = 12.3456;

  // Output formatted strings
  printf ("a) Output a string: %s\n", a0chS);
  printf ("b) Output a character: %c\n", chC);
  printf ("   -> As integer: %d\n", chC);
  printf ("c) Output an integer: %d\n", inI);
  printf ("   -> With seven characters: %7d\n", inI);
  printf ("   -> With seven characters and leading zeros: %07d\n", inI);
  printf ("d) Output a float: %f\n", sfF);
  printf ("   -> With six characters and two decimal places: %6.2f\n", sfF);

  return 0;
}
