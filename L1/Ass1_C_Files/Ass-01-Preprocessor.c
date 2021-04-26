/*        $Id: Ass-01-Preprocessor.c 4642 2021-02-12 12:57:13Z rbetz $
 *  $Revision: 4642 $
 *      $Date: 2021-02-12 23:57:13 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>

int finPreprocessor_main (int argc, char *argv[])
{

// Conditional statement
  printf ("a) Conditional statement\n");
  printf ("   Check if DEF1 has been defined:\n");
#ifdef DEF1
  printf ("   -> DEF1 has been defined\n");
#else
  printf ("   -> DEF1 has not been defined\n");
#endif

// Include statement
// Note that DEF1 is defined in LabEx-Preprocessor.h
  printf ("b) Include statement\n");
  printf ("   Include LabEx-Preprocessor.h (which defines DEF1)\n");
#include "Ass-01-Preprocessor.h"

  // Conditional statement for value
  printf ("c) Conditional statement for value\n");
  printf ("   Check value of DEF2:\n");
#if DEF2 == 1234
  printf ("   -> DEF2 = 1234\n");
#else
  printf ("   -> DEF2 does not equal 1234\n");
#endif

// Include statement
// Note that LabEx-Preprocessor.h is included again
  printf ("d) Include statement to check for reinclusion\n");
  printf ("   Include LabEx-Preprocessor.h again (which defines DEF1)\n");
#include "Ass-01-Preprocessor.h"
  printf ("   Has DEF1 been defined more than once:\n");
  printf ("   -> %s\n", DEF1_DEFINED_TWICE);

// Use defines where numbers are used in more that one place
  printf ("e) Use defines rather than literals in more that once place\n");
  printf ("   DEF2 used in two places:\n");
  printf ("   -> DEF2 = %d\n", DEF2);
  printf ("   -> DEF2*2 = %d\n", DEF2 * 2);

// Be careful with complex definitions
// Use parenthesis since don't know where definition will be used
#define DEF3_BAD 1+2
#define DEF3_GOOD (1+2)
  printf ("f) Complex definitions\n");
  printf ("   Be careful with complex definitions:\n");
  printf ("   -> DEF3_BAD = %d\n", DEF3_BAD);
  printf ("   -> DEF3_GOOD = %d\n", DEF3_GOOD);
  printf ("   -> DEF3_BAD*3 = %d\n", DEF3_BAD * 3);
  printf ("   -> DEF3_GOOD*3 = %d\n", DEF3_GOOD * 3);

// Defining a macro that passes arguments
  {
    printf ("g) Macro that passes parameters\n");
#define min(x,y) ((x)<(y)?(x):(y))
    int a = 7;
    int b = 10;
    printf ("   a = %d, b = %d\n", a, b);
    printf ("   -> min(a,b) = %d\n", min(a, b));
    printf ("   -> min(3+5,b) = %d\n", min(3 + 5, b));
  }

  return 0;
}
