/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* mpn_addmul_1 -- multiply the S1_SIZE long limb vector pointed to by S1_PTR
   by S2_LIMB, add the S1_SIZE least significant limbs of the product to the
   limb vector pointed to by RES_PTR.  Return the most significant limb of
   the product, adjusted for carry-out from the addition.

Copyright (C) 1992, 1993, 1994, 1996 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include <gmp.h>
#include "gmp-impl.h"
#include "longlong.h"

mp_limb_t
mpn_addmul_1 (res_ptr, s1_ptr, s1_size, s2_limb)
     register mp_ptr res_ptr;
     register mp_srcptr s1_ptr;
     mp_size_t s1_size;
     register mp_limb_t s2_limb;
{
  register mp_limb_t cy_limb;
  register mp_size_t j;
  register mp_limb_t prod_high, prod_low;
  register mp_limb_t x;

  x = res_ptr[0];              /* откручена первая итерация */
  umul_ppmm (prod_high, prod_low, s1_ptr[0], s2_limb);

#if __iset__ < 5
  prod_low = x + prod_low;
  cy_limb = prod_high + (prod_low < x);
#else /* __iset__ < 5 */
  cy_limb = prod_high + __builtin_e2k_addcd_c (x, prod_low, 0);
  prod_low = x + prod_low;
#endif /* __iset__ < 5 */
  res_ptr[0] = prod_low;

  if (1 >= s1_size) return cy_limb;
  {           /* откручена вторая итерация */
      x = res_ptr[1];
      umul_ppmm (prod_high, prod_low, s1_ptr[1], s2_limb);

#if __iset__ < 5
      prod_low += cy_limb;
      cy_limb = (prod_low < cy_limb) + prod_high;

      prod_low = x + prod_low;
      cy_limb += (prod_low < x);
#else /* __iset__ < 5 */
      int carry1 = __builtin_e2k_addcd_c (prod_low, cy_limb, 0);
      prod_low += cy_limb;
      int carry2 = __builtin_e2k_addcd_c (prod_low, x, 0);
      prod_low = x + prod_low;
      cy_limb = __builtin_e2k_addcd (prod_high, carry1, carry2);
#endif /* __iset__ < 5 */
      res_ptr[1] = prod_low;
  }
  if (2 >= s1_size) return cy_limb;

#pragma loop count (2)
  for (j = 2; j < s1_size; j++) {
      x = res_ptr[j];
      umul_ppmm (prod_high, prod_low, s1_ptr[j], s2_limb);

#if __iset__ < 5
      prod_low += cy_limb;
      cy_limb = (prod_low < cy_limb) + prod_high;

      prod_low = x + prod_low;
      cy_limb += (prod_low < x);
#else /* __iset__ < 5 */
      int carry1 = __builtin_e2k_addcd_c (prod_low, cy_limb, 0);
      prod_low += cy_limb;
      int carry2 = __builtin_e2k_addcd_c (prod_low, x, 0);
      prod_low = x + prod_low;
      cy_limb = __builtin_e2k_addcd (prod_high, carry1, carry2);
#endif /* __iset__ < 5 */
      res_ptr[j] = prod_low;
  }
  return cy_limb;
}
