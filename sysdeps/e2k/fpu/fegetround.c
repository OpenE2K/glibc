/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Return current rounding direction. */

#include <fenv.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__fegetround (void)
{
  unsigned int pfpfr;

  __fenv_getpfpfr (pfpfr);

  return pfpfr & __FE_ROUND_MASK;
}

libm_hidden_def (__fegetround)
weak_alias (__fegetround, fegetround)
libm_hidden_weak (fegetround)
