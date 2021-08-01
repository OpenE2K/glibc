/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Return minimum numeric value of X and Y.
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

float
__fminf (float x, float y)
{
  _type_float_bits Y;

  Y.value = y;
  if (__glibc_unlikely ((Y.int0 & 0x7fffffff) > 0x7f800000)) /* y is NaN  */
      return x;
#pragma asm_inline
  __asm ("fmins %0,%1,%0" : "+r" (x) : "r" (y));
  return x;
}

weak_alias (__fminf, fminf)
