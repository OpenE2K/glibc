/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <limits.h>
#include <math.h>
#include <math_private.h>

int __ieee754_ilogbf(float x)
{
  __extension__ union { float __f; int __i; } __xx = { __f: x };
  int __ix = __xx.__i & 0x7fffffff;

  if (__ix == 0)
    return FP_ILOGB0;   /* ilogb(0) = FP_ILOGB0 */
  if (__glibc_unlikely (__ix < 0x00800000)) {  /* subnormal x */
      return 9 - __builtin_clz (__ix) - 127;
  }
  if (__glibc_likely (__ix < 0x7f800000))
      return (__ix >> 23) - 127;
  if (FP_ILOGBNAN != INT_MAX) {
      /* ISO C99 requires ilogbf(+-Inf) == INT_MAX.  */
      if (__ix == 0x7f800000)
          return INT_MAX;
  }
  return FP_ILOGBNAN;
}
