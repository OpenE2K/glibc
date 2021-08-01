/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ğòïçòáííá  ÷ïú÷òáıáåô äòïâîõà şáóôø áòçõíåîôá , á  %
%  ãåìáñ şáóôø áòçõíåîôá ğïíåıáåôóñ ÷ "*IòTR".           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

double
__modf (double x, double *iptr)
{
  _type_double_bits X, Y, sign;
  DB tmp;
  LL ix;

  X.value = x;
  ix = X.llong & 0x7fffffffffffffffLL;
  sign.llong = X.llong & ~0x7fffffffffffffffLL;    /* sign of x */

  if (__glibc_unlikely (ix >= 0x4330000000000000LL)) { /* |x| >= 2^52, inf, nan  */
      *iptr = x;
      if (ix > 0x7ff0000000000000LL) /* We must handle NaNs separately.  */
          return x * 1.0;
      return sign.value; /* +- 0 */
  }

#if __iset__ <= 2
  tmp = (DB) (LL) x;
#else /*  __iset__ <= 2 */
_Pragma ("asm_inline")
  __asm ("fdtoifd 0x3,%1,%0" : "=r" (tmp): "r" (x));
#endif /*  __iset__ <= 2 */
  Y.value = tmp;
  Y.llong |= sign.llong;
  *iptr = Y.value;                /* integer part of x */
  X.value = x - tmp;
  X.llong |= sign.llong;
  return X.value;                /* fraction part of x */
}
weak_alias (__modf, modf)
#ifdef NO_LONG_DOUBLE
strong_alias (__modf, __modfl)
weak_alias (__modf, modfl)
#endif
