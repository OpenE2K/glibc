/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/*
 * isnanf(x) returns 1 is x is nan, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__isnanf (float x)
{
  return isunordered (x, x);
}

hidden_def (__isnanf)
weak_alias (__isnanf, isnanf)
