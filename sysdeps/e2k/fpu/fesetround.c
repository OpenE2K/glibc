/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Set current rounding direction.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <fenv.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__fesetround (int round)
{
  unsigned int pfpfr, fpcr;

  if ((round & ~__FE_ROUND_MASK) != 0)
    /* ROUND is no valid rounding mode.  */
    return 1;

  __fenv_getpfpfr (pfpfr);
  __fenv_getfpcr (fpcr);

  pfpfr &= ~__FE_ROUND_MASK;
  pfpfr |= round;

  fpcr &= ~(__FE_ROUND_MASK >> 3);
  fpcr |= (round >> 3);

  __fenv_setpfpfr (pfpfr);
  __fenv_setfpcr (fpcr);

  return 0;
}

libm_hidden_def (__fesetround)
weak_alias (__fesetround, fesetround)
libm_hidden_weak (fesetround)
