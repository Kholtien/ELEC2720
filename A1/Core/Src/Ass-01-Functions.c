/*        $Id: Ass-01-Functions.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Function Prototypes
static int16_t fsinMySquareRootArray(int16_t a0sinX[], int16_t a0sinY[], uint16_t usinA_size);

// Define return status values for mySquareRootArray

#define SRA_OK 0
#define SRA_ERR_ZERO_SIZE 1
#define SRA_ERR_NEG_VALUE 2

static int16_t fsinMySquareRootArray(int16_t *sinA_out, int16_t sinA_in[], uint16_t usinA_size)
{
  uint16_t usinI;

  if (usinA_size == 0)
  {
    return SRA_ERR_ZERO_SIZE;
  }

  for (usinI = 0; usinI < usinA_size; usinI++)
  {
    if (sinA_in[usinI] < 0)
    {
      return SRA_ERR_NEG_VALUE;
    }
    sinA_out[usinI] = sqrt (sinA_in[usinI]);
  }

  return SRA_OK;
}

int finFunctions_main (int argc, char *argv[])
{

#define ARRAY_SIZE 3
  int16_t a0sinA[ARRAY_SIZE] = { 1, 4, 9 };
  int16_t a0sinB[ARRAY_SIZE];
  int16_t sinS;
  uint16_t usinI;

  printf ("a) Valid input data:\n");
  if ((sinS = fsinMySquareRootArray(a0sinB, a0sinA, ARRAY_SIZE)) == SRA_OK)
  {
    for (usinI = 0; usinI < ARRAY_SIZE; usinI++)
    {
      printf ("   sqrt(%d) = %d\n", a0sinA[usinI], a0sinB[usinI]);
    }
  }
  else
  {
    printf ("   ERROR: Return status = %d\n", sinS);
  }

  printf ("b) Zero size data:\n");
  if ((sinS = fsinMySquareRootArray(a0sinB, a0sinA, 0)) == SRA_OK)
  {
    for (usinI = 0; usinI < ARRAY_SIZE; usinI++)
    {
      printf ("   sqrt(%d) = %d\n", a0sinA[usinI], a0sinB[usinI]);
    }
  }
  else
  {
    printf ("   ERROR: Return status = %d\n", sinS);
  }

  printf ("c) Negative input data:\n");
  a0sinA[1] = -a0sinA[1]; // Negate one input
  if ((sinS = fsinMySquareRootArray (a0sinB, a0sinA, ARRAY_SIZE)) == SRA_OK)
  {
    for (usinI = 0; usinI < ARRAY_SIZE; usinI++)
    {
      printf ("   sqrt(%d) = %d\n", a0sinA[usinI], a0sinB[usinI]);
    }
  }
  else
  {
    printf ("   ERROR: Return status = %d\n", sinS);
  }

  return 0;
}
