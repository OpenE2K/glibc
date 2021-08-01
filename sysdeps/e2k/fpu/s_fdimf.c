/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Return positive difference between arguments.
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

#include <errno.h>
#include "f2c.h"

float
__fdimf (float x, float y)
{
  _type_float_bits X, Y;
  int ix, iy;

  X.value = x;
  ix = X.int0 & 0x7fffffff;
  Y.value = y;
  iy = Y.int0 & 0x7fffffff;

  if (ix > 0x7f800000 || iy > 0x7f800000) /* x or y is NaN */
      /* Raise invalid flag for signaling but not quiet NaN.  */
      return x - y;

  if (x <= y)
    return 0;

  X.value = x - y;
  if ((X.int0 & 0x7fffffff) == 0x7f800000 &&
    ix != 0x7f800000 && iy != 0x7f800000)
    __set_errno (ERANGE);

  return X.value;
}

weak_alias (__fdimf, fdimf)
