/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * isnan(x) returns 1 is x is nan, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__isnan (double x)
{
  return isunordered (x, x);
}

hidden_def (__isnan)
weak_alias (__isnan, isnan)
#ifdef NO_LONG_DOUBLE
strong_alias (__isnan, __isnanl)
weak_alias (__isnan, isnanl)
#endif
