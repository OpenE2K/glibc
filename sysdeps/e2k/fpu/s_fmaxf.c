/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Return maximum numeric value of X and Y. */

#include "f2c.h"

float
__fmaxf (float x, float y)
{
  _type_float_bits Y;

  Y.value = y;
  if (__glibc_unlikely ((Y.int0 & 0x7fffffff) > 0x7f800000)) /* y is NaN  */
      return x;
#pragma asm_inline
  __asm ("fmaxs %0,%1,%0" : "+r" (x) : "r" (y));
  return x;
}

weak_alias (__fmaxf, fmaxf)
