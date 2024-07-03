/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Test exception in current environment. */

#include <fenv.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
fetestexcept (int excepts)
{
  unsigned int pfpfr, fpsr;

  __fenv_getpfpfr (pfpfr);
  __fenv_getfpsr (fpsr);

  return (pfpfr | fpsr) & (excepts & FE_ALL_EXCEPT);
}

libm_hidden_def (fetestexcept)
