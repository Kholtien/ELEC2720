/*        $Id: Ass-01-Preprocessor.h 4638 2021-02-11 23:26:26Z rbetz $
 *  $Revision: 4638 $
 *      $Date: 2021-02-12 10:26:26 +1100 (Fri, 12 Feb 2021) $
 *    $Author: rbetz $
 */

#ifndef ASS_01_PREPROCESSOR_H_
#define ASS_01_PREPROCESSOR_H_

// Test if DEF1 gets defined twice
#ifdef DEF1
#define DEF1_DEFINED_TWICE "Yes"
#else
#define DEF1_DEFINED_TWICE "No"
#endif

// Make some definitions
#define DEF1       // Defined only
#define DEF2 1234  // Defined to be a value

#endif /* ASS_01_PREPROCESSOR_H_ */
