/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Return maximum numeric value of X and Y. */

#include "f2c.h"

double
__fmax (double x, double y)
{
  _type_double_bits Y;

  Y.value = y;
  if (__glibc_unlikely ((Y.llong & 0x7fffffffffffffffLL) > 0x7ff0000000000000LL)) /* y is NaN  */
      return x;
#pragma asm_inline
  __asm ("fmaxd %0,%1,%0" : "+r" (x) : "r" (y));
  return x;
}

weak_alias (__fmax, fmax)
#ifdef NO_LONG_DOUBLE
strong_alias (__fmax, __fmaxl)
weak_alias (__fmax, fmaxl)
#endif
