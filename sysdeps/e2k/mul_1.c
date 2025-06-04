/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* mpn_mul_1 -- Multiply a limb vector with a single limb and
   store the product in a second limb vector. */

#include <gmp.h>
#include "gmp-impl.h"
#include "longlong.h"

mp_limb_t
mpn_mul_1 (register mp_ptr res_ptr,
	   register mp_srcptr s1_ptr,
	   mp_size_t s1_size,
	   register mp_limb_t s2_limb)
{
  register mp_limb_t cy_limb;
  register mp_size_t j;
  register mp_limb_t prod_high, prod_low;
  register mp_limb_t x;

  umul_ppmm (prod_high, prod_low, s1_ptr[0], s2_limb);   /* откручена первая итерация */
  cy_limb = prod_high;
  if (!(1 >= s1_size))
      x = s1_ptr[1];
  res_ptr[0] = prod_low;

  if (1 >= s1_size) return cy_limb;
  {           /* откручена вторая итерация */
    umul_ppmm (prod_high, prod_low, x, s2_limb);

#if __iset__ < 5
    prod_low += cy_limb;
    cy_limb = (prod_low < cy_limb) + prod_high;
#else /* __iset__ < 5 */
    int carry = __builtin_e2k_addcd_c (cy_limb, prod_low, 0);
    prod_low += cy_limb;
    cy_limb = __builtin_e2k_addcd (prod_high, 0, carry);
#endif /* __iset__ < 5 */
    res_ptr[1] = prod_low;
  }
  if (2 >= s1_size) return cy_limb;

#pragma loop count (2)
  for (j = 2; j < s1_size; j++) {
    umul_ppmm (prod_high, prod_low, s1_ptr[j], s2_limb);

#if __iset__ < 5
    prod_low += cy_limb;
    cy_limb = (prod_low < cy_limb) + prod_high;
#else /* __iset__ < 5 */
    int carry = __builtin_e2k_addcd_c (cy_limb, prod_low, 0);
    prod_low += cy_limb;
    cy_limb = __builtin_e2k_addcd (prod_high, 0, carry);
#endif /* __iset__ < 5 */
    res_ptr[j] = prod_low;
  }
  return cy_limb;
}
