/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Set current rounding direction. */

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
