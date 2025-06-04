/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * finitef(x) returns 1 is x is finite, else 0;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__finitef (float x)
{
  return (__extension__ (((union { float __f; int __i; })
    {__f: x}).__i & 0x7fffffff) < 0x7f800000);
}

hidden_def (__finitef)
weak_alias (__finitef, finitef)
