/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/*
 * finitel(x) returns 1 is x is finite, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__finitel (long double x)
{
  return (__extension__ (((union { long double __f; struct { long long __ll; short __si; } twolong; })
    {__f: x}).twolong.__si & 0x7fff) < 0x7fff);
}

hidden_def (__finitel)
weak_alias (__finitel, finitel)
