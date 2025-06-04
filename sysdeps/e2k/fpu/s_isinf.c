/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * isinf(x) returns 1 is x is inf, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__isinf (double x)
{
  __extension__ union { double __f; long long __i; } __u = { __f: x };
  return (__u.__i & 0x7fffffffffffffffLL) == 0x7ff0000000000000LL ?
      (__u.__i >> 62) : 0;
}

hidden_def (__isinf)
weak_alias (__isinf, isinf)
#ifdef NO_LONG_DOUBLE
strong_alias (__isinf, __isinfl)
weak_alias (__isinf, isinfl)
#endif
