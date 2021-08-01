/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * isinff(x) returns 1 is x is inf, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__isinff (float x)
{
  __extension__ union { float __f; int __i; } __u = { __f: x };
  return (__u.__i & 0x7fffffff) == 0x7f800000 ? (__u.__i >> 30) : 0;
}

hidden_def (__isinff)
weak_alias (__isinff, isinff)
