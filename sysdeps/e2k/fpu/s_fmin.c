/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Return minimum numeric value of X and Y. */

#include "f2c.h"

double
__fmin (double x, double y)
{
  _type_double_bits Y;

  Y.value = y;
  if (__glibc_unlikely ((Y.llong & 0x7fffffffffffffffLL) > 0x7ff0000000000000LL)) /* y is NaN  */
      return x;
#pragma asm_inline
  __asm ("fmind %0,%1,%0" : "+r" (x) : "r" (y));
  return x;
}

weak_alias (__fmin, fmin)
#ifdef NO_LONG_DOUBLE
strong_alias (__fmin, __fminl)
weak_alias (__fmin, fminl)
#endif
