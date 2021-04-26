/*        $Id: Ass-01-MemoryManagement.c 4642 2021-02-12 12:57:13Z rbetz $
 *  $Revision: 4642 $
 *      $Date: 2021-02-12 23:57:13 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int16_t sinM; // Module level

static void fvdTestStatic () // This function only visible in this module
{
  static int16_t sinS = 0;

  if (sinS == 0)
  {
    printf ("   &sinS = %p\n", &sinS);
  }
  printf ("   sinS = %d\n", sinS);
  sinS++;
}

int finMemoryManagement_main (int argc, char *argv[])
{

  int16_t sinA; // Local to function

  printf ("a) Module and local variables:\n");
  printf ("   &sinM = %p\n", &sinM);
  printf ("   &sinA = %p\n", &sinA);

  printf ("b) Scope:\n");
  {
    int16_t sinM; // Module level
    int16_t sinA; // Local to function

    printf ("   &sinM = %p\n", &sinM);
    printf ("   &sinA = %p\n", &sinA);
  }

  printf ("c) Static variable:\n");
  fvdTestStatic ();
  fvdTestStatic ();
  fvdTestStatic ();

  return 0;
}
