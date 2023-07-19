/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Return maximum numeric value of X and Y.
   Copyright (C) 1997-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "f2c.h"

double
__fmax (double x, double y)
{
  _type_double_bits Y;

  Y.value = y;
  if (__glibc_unlikely ((Y.llong & 0x7fffffffffffffffLL) > 0x7ff0000000000000LL)) /* y is NaN  */
      return x;
#pragma asm_inline
  __asm ("fmaxd %0,%1,%0" : "+r" (x) : "r" (y));
  return x;
}

weak_alias (__fmax, fmax)
#ifdef NO_LONG_DOUBLE
strong_alias (__fmax, __fmaxl)
weak_alias (__fmax, fmaxl)
#endif
