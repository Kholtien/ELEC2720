/*        $Id: Ass-01-PassingParameters.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>

int finPassingParameters_main (int argc, char *argv[])
{
  int inI;
  printf ("Number of arguments passed = %d\n", argc);
  for (inI = 0; inI < argc; inI++)
  {
    printf ("-> Argument number %d is %s\n",inI + 1, argv[inI]);
  }
  return 0;
}
