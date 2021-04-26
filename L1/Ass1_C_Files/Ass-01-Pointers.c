/*        $Id: Ass-01-Pointers.c 4655 2021-02-14 07:29:57Z rbetz $
 *  $Revision: 4655 $
 *      $Date: 2021-02-14 18:29:57 +1100 (Sun, 14 Feb 2021) $
 *    $Author: rbetz $
 */

#include <stdio.h>
#include <stdint.h>

int finPointers_main (int argc, char *argv[])
{
  int16_t sinA;    // Declare sinA int16_t
  int16_t *psinA; // Declare a pointer to a int16_t (use *)

  printf ("a) Assigning a pointer:\n");
  sinA = 1234;
  psinA = &sinA; // Assign to be the address of sinA (use &)
  printf ("   sinA = %d, *psinA_p = %d, psinA_p = %p, &sinA = %p\n", sinA, *psinA, psinA, &sinA);
  printf ("b) Dereferencing a pointer:\n");
  *psinA = 5678; // Set value of psinA by dereferencing pointer to psinA (use *)
  printf ("   sinA = %d, *psinA_p = %d, psinA_p = %p, &sinA = %p\n", sinA, *psinA, psinA, &sinA);

  return 0;
}
