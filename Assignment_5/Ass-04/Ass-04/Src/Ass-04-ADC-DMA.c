/*        $Id: Ass-04-ADC-DMA.c 1863 2019-05-13 23:59:49Z Peter $
 *  $Revision: 1863 $
 *      $Date: 2019-05-14 09:59:49 +1000 (Tue, 14 May 2019) $
 *    $Author: Peter $
 */

#include "Ass-04.h"

/*
 * The ADC has been set up to sequence both analog inputs.
 * The interrupt is at the end of the sequence after both conversions.
 * DMA transfer the result of each conversion.
 */

void
ADC_DMA (void)
{

  printf ("ADC_DMA():\n");

}
