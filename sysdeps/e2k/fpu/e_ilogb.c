/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* ilogb(double x)
 * return the binary exponent of non-zero x
 * ilogb(0) = FP_ILOGB0
 * ilogb(NaN) = FP_ILOGBNAN (no signal is raised)
 * ilogb(+-Inf) = INT_MAX (no signal is raised)
 */

#include <limits.h>
#include <math.h>
#include <math_private.h>

#include "f2c.h"

int
__ieee754_ilogb (double x)
{
  __extension__ union { double __f; long long __i; } __xx = { __f: x };
  LL __ix = __xx.__i & 0x7fffffffffffffffLL;
  int __ex = __ix >> 52;

  if (__ix == 0)
    return FP_ILOGB0;   /* ilogb(0) = FP_ILOGB0 */
  if (__glibc_unlikely (__ex == 0)) {  /* subnormal x */
      return  12 - __builtin_clzll (__ix) - 1023;
  }
  if (__glibc_likely (__ex < 0x7ff))
      return __ex - 1023;
  if (FP_ILOGBNAN != INT_MAX) {
      /* ISO C99 requires ilogbf(+-Inf) == INT_MAX.  */
      if (__ix == 0x7ff0000000000000LL)
          return INT_MAX;
  }
  return FP_ILOGBNAN;
}
