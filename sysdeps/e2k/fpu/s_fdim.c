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

double
__fdim (double x, double y)
{
  _type_double_bits X, Y;
  LL ix, iy;

  X.value = x;
  ix = X.llong & 0x7fffffffffffffffLL;
  Y.value = y;
  iy = Y.llong & 0x7fffffffffffffffLL;

  if (ix > 0x7ff0000000000000LL || iy > 0x7ff0000000000000LL) /* x or y is NaN */
      /* Raise invalid flag for signaling but not quiet NaN.  */
      return x - y;

  if (x <= y)
    return 0.0;

  X.value = x - y;
  if ((X.llong & 0x7fffffffffffffffLL) == 0x7ff0000000000000LL &&
    ix != 0x7ff0000000000000LL && iy != 0x7ff0000000000000LL)
    __set_errno (ERANGE);

  return X.value;
}

weak_alias (__fdim, fdim)
#ifdef NO_LONG_DOUBLE
strong_alias (__fdim, __fdiml)
weak_alias (__fdim, fdiml)
#endif
