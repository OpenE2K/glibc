/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * finite(x) returns 1 is x is finite, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__finite (double x)
{
  return (__extension__ (((union { double __f; long long __i; })
    {__f: x}).__i & 0x7fffffffffffffffLL) < 0x7ff0000000000000LL);
}

hidden_def (__finite)
weak_alias (__finite, finite)
#ifdef NO_LONG_DOUBLE
strong_alias (__finite, __finitel)
weak_alias (__finite, finitel)
#endif
