/* Copyright (C) 1995-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gnu.ai.mit.edu>, August 1995.

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

#include <ieee754.h>
#include <stdlib.h>
#include <limits.h>

#ifdef DONT_USE_FAST_DRAND48

int
__erand48_r (unsigned short int xsubi[3], struct drand48_data *buffer,
	     double *result)
{
  union ieee754_double temp;

  /* Compute next state.  */
  if (__drand48_iterate (xsubi, buffer) < 0)
    return -1;

  /* Construct a positive double with the 48 random bits distributed over
     its fractional part so the resulting FP number is [0.0,1.0).  */

  temp.ieee.negative = 0;
  temp.ieee.exponent = IEEE754_DOUBLE_BIAS;
  temp.ieee.mantissa0 = (xsubi[2] << 4) | (xsubi[1] >> 12);
  temp.ieee.mantissa1 = ((xsubi[1] & 0xfff) << 20) | (xsubi[0] << 4);

  /* Please note the lower 4 bits of mantissa1 are always 0.  */
  *result = temp.d - 1.0;

  return 0;
}

#else /* version for e2k */

int
__erand48_r (unsigned short int xsubi[3], struct drand48_data *buffer,
	     double *dresult)
{
  union ieee754_double temp;
  uint64_t X;
  uint64_t result;

  /* Do the real work.  We choose a data type which contains at least
     48 bits.  Because we compute the modulus it does not care how
     many bits really are computed.  */

  X = (uint64_t) xsubi[2] << 32 | (uint32_t) xsubi[1] << 16 | xsubi[0];
  result = X * buffer->__a + buffer->__c;

  /* Initialize buffer, if not yet done.  */
  if (__glibc_unlikely (!buffer->__init))
    {
      buffer->__a = 0x5deece66dull;
      buffer->__c = 0xb;
      buffer->__init = 1;
      result = X * buffer->__a + buffer->__c;
    }

  xsubi[0] = result & 0xffff;
  xsubi[1] = (result >> 16) & 0xffff;
  xsubi[2] = (result >> 32) & 0xffff;

  /* Construct a positive double with the 48 random bits distributed over
     its fractional part so the resulting FP number is [0.0,1.0).  */

  *((uint64_t *)&temp) = ((result & 0xffffffffffffLL) << 4) | 0x3ff0000000000000LL;

  /* Please note the lower 4 bits of mantissa1 are always 0.  */
  *dresult = temp.d - 1.0;

  return 0;
}


/* for __erand48_r inline into drand48/erand48 */
#define DONT_USE_FAST_DRAND48
#include "drand48.c"
#include "erand48.c"
#include "drand48-iter.c"

#endif

weak_alias (__erand48_r, erand48_r)
