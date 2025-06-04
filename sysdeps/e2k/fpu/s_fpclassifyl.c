/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * fpclassifyl(x) returns classification value corresponding to argument;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__fpclassifyl (long double x)
{
  __extension__ union { long double __f; struct { long long __ll; short __si; } twolong; } __xx = { __f: x };
  int __retval = FP_NORMAL;
  int __ex = __xx.twolong.__si & 0x7fff;
  long long __m = __xx.twolong.__ll & ~0x8000000000000000LL;

  if (__ex == 0) {
      if (__xx.twolong.__ll == 0)
        __retval = FP_ZERO;
      else if (__xx.twolong.__ll == __m)
        __retval = FP_SUBNORMAL;
  }
  else if (__ex == 0x7fff)
    __retval = __m == 0 ? FP_INFINITE : FP_NAN;
  return __retval;
}

hidden_def (__fpclassifyl)
