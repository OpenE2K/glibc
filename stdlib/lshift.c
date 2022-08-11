/* mpn_lshift -- Shift left low level.

Copyright (C) 1991-2019 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, see
<http://www.gnu.org/licenses/>.  */

#include <gmp.h>
#include "gmp-impl.h"

/* This lets us inhibit assert () below in this file for the sake of
   performance. For additional explanations see Bug #89674.  */
#if defined __e2k__ && defined __LCC__
#ifndef NDEBUG
# define NDEBUG			/* Undefine this for debugging assertions.  */
#endif
#endif /* defined __e2k__ && defined __LCC__  */
#include <assert.h>

/* Shift U (pointed to by UP and USIZE digits long) CNT bits to the left
   and store the USIZE least significant digits of the result at WP.
   Return the bits shifted out from the most significant digit.

   Argument constraints:
   1. 0 < CNT < BITS_PER_MP_LIMB
   2. If the result is to be written over the input, WP must be >= UP.
*/

mp_limb_t
mpn_lshift (register mp_ptr wp,
	    register mp_srcptr up, mp_size_t usize,
	    register unsigned int cnt)
{
  register mp_limb_t high_limb, low_limb;
  register unsigned sh_1, sh_2;
  register mp_size_t i;
  mp_limb_t retval;

  assert (usize != 0 && cnt != 0);

  sh_1 = cnt;
#if 0
  if (sh_1 == 0)
    {
      if (wp != up)
	{
	  /* Copy from high end to low end, to allow specified input/output
	     overlapping.  */
	  for (i = usize - 1; i >= 0; i--)
	    wp[i] = up[i];
	}
      return 0;
    }
#endif

  wp += 1;
  sh_2 = BITS_PER_MP_LIMB - sh_1;
  i = usize - 1;
  low_limb = up[i];
  retval = low_limb >> sh_2;
  high_limb = low_limb;
#if defined __e2k__ && defined __LCC__
#pragma loop count (1)
#endif /* defined __e2k__ && defined __LCC__  */
  while (--i >= 0)
    {
      low_limb = up[i];
      wp[i] = (high_limb << sh_1) | (low_limb >> sh_2);
      high_limb = low_limb;
    }
  wp[i] = high_limb << sh_1;

  return retval;
}
