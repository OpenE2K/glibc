/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* mpn_sub_n -- Subtract two limb vectors of equal, non-zero length. */

#include <gmp.h>
#include "gmp-impl.h"

mp_limb_t
#if __STDC__
mpn_sub_n (mp_ptr __restrict res_ptr, mp_srcptr __restrict s1_ptr, mp_srcptr __restrict s2_ptr, mp_size_t size)
#else
mpn_sub_n (res_ptr, s1_ptr, s2_ptr, size)
     register mp_ptr res_ptr;
     register mp_srcptr s1_ptr;
     register mp_srcptr s2_ptr;
     mp_size_t size;
#endif
{
  register mp_limb_t x, y, z, cy;
  register mp_size_t j;

  x = s1_ptr[0];              /* откручена первая итерация */
  y = s2_ptr[0];
  z = x - y;                  /* main subtract */
  res_ptr[0] = z;
#if __iset__ < 5
  cy = (y > x);               /* get out carry from the subtract */
  if (size <= 1) return cy;

#pragma loop count (2)
  for (j = 1; j < size; j++) {
    x = s1_ptr[j];
    y = s2_ptr[j];
    y += cy;                  /* add previous carry to subtrahend */
    z = x - y;                /* main subtract */
    cy = (y < cy) || (y > x);  /* get out carry from the subtract, combine */
    res_ptr[j] = z;
  }
#else /* __iset__ < 5 */
  cy = __builtin_e2k_subcd_c (x, y, 0);
  if (size <= 1) return cy;

#pragma loop count (2)
  for (j = 1; j < size; j++) {
    x = s1_ptr[j];
    y = s2_ptr[j];
    z = __builtin_e2k_subcd (x, y, cy);
    cy = __builtin_e2k_subcd_c (x, y, cy);
    res_ptr[j] = z;
  }
#endif /* __iset__ < 5 */
  return cy;
}
