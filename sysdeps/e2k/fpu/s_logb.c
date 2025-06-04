/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

double
__logb (double x)
{
  __extension__ union { double __f; long long __i; } __xx = { __f: x };
  LL __ix = __xx.__i & 0x7fffffffffffffffLL;
  int __ex = __ix >> 52;

  if (__ix == 0)
    return -1.0 / dfabs (x); /* -inf */
  if (__glibc_unlikely (__ex == 0))
    {
      /* POSIX specifies that denormal number is treated as
         though it were normalized.  */
      return (double) (12 - __builtin_clzll (__ix) - 1023);
    }
  if (__glibc_unlikely (__ex == 0x7ff))
    return x * x; /* +inf or nan */
  return (double) (__ex - 1023);
}

weak_alias (__logb, logb)
#ifdef NO_LONG_DOUBLE
strong_alias (__logb, __logbl) weak_alias (__logb, logbl)
#endif
