/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* mpn_add_n -- Add two limb vectors of equal, non-zero length. */

#include <gmp.h>
#include "gmp-impl.h"

mp_limb_t
#if __STDC__
mpn_add_n (mp_ptr res_ptr, mp_srcptr s1_ptr, mp_srcptr s2_ptr, mp_size_t size)
#else
mpn_add_n (res_ptr, s1_ptr, s2_ptr, size)
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
  z = x + y;                  /* add other addend */
  res_ptr[0] = z;
#if __iset__ < 5
  cy = (z < x);               /* get out carry from that add, combine */
  if (size <= 1) return cy;

#pragma loop count (2)
  for (j = 1; j < size; j++) {
    x = s1_ptr[j];
    y = s2_ptr[j];
    y += cy;                  /* add previous carry to one addend */
    z = x + y;                /* add other addend */
    cy = (y < cy) || (z < x);  /* get out carry from that add, combine */
    res_ptr[j] = z;
  }
#else /* __iset__ < 5 */
  cy = __builtin_e2k_addcd_c (x, y, 0);
  if (size <= 1) return cy;

#pragma loop count (2)
  for (j = 1; j < size; j++) {
    x = s1_ptr[j];
    y = s2_ptr[j];
    z = __builtin_e2k_addcd (x, y, cy);
    cy = __builtin_e2k_addcd_c (x, y, cy);
    res_ptr[j] = z;
  }
#endif /* __iset__ < 5 */
  return cy;
}
