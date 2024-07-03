/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

float
__logbf (float x)
{
  __extension__ union { float __f; int __i; } __xx = { __f: x };
  int __ix = __xx.__i & 0x7fffffff;

  if (__ix == 0)
    return -1.0f / ffabs (x); /* -inf */
  if (__glibc_unlikely (__ix < 0x00800000))
    {
      /* POSIX specifies that denormal number is treated as
         though it were normalized.  */
       return (float) (9 - __builtin_clz (__ix) - 127);
    }
  if (__glibc_unlikely (__ix >= 0x7f800000))
    return x * x; /* +inf or nan */
  return (float) ((__ix >> 23) - 127);
}

weak_alias (__logbf, logbf)
