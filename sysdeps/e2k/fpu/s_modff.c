/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ğòïçòáííá  ÷ïú÷òáıáåô äòïâîõà şáóôø áòçõíåîôá , á  %
%  ãåìáñ şáóôø áòçõíåîôá ğïíåıáåôóñ ÷ "*IòTR".           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

float
__modff(float x, float *iptr)
{
  _type_float_bits X, Y, sign;
  FL tmp;
  int ix;

  X.value = x;
  ix = X.int0 & 0x7fffffff;
  sign.int0 = X.int0 & ~0x7fffffff;    /* sign of x */

  if (__glibc_unlikely (ix >= 0x4b000000)) { /* |x| >= 2^23, inf, nan  */
      *iptr = x;
      if (ix > 0x7f800000) /* We must handle NaNs separately.  */
          return x * 1.0F;
      return sign.value; /* +- 0 */
  }

#if __iset__ <= 2
  tmp = (FL) (int) x;
#else /*  __iset__ <= 2 */
_Pragma ("asm_inline")
  __asm ("fstoifs 0x3,%1,%0" : "=r" (tmp): "r" (x));
#endif /*  __iset__ <= 2 */
  Y.value = tmp;
  Y.int0 |= sign.int0;
  *iptr = Y.value;                /* integer part of x */
  X.value = x - tmp;
  X.int0 |= sign.int0;
  return X.value;                /* fraction part of x */
}

weak_alias (__modff, modff)
