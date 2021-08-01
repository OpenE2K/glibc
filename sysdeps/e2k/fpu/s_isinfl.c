/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * isinfl(x) returns 1 is x is inf, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__isinfl (long double x)
{
  __extension__ union { long double __f; short __i[8]; long long __ll; } __u = { __f: x };
  return (((__u.__i[4] & 0x7fff) == 0x7fff
	   && __u.__ll == 0x8000000000000000LL) ?
	  __u.__i[4] >> 14 : 0);
}

hidden_def (__isinfl)
weak_alias (__isinfl, isinfl)
