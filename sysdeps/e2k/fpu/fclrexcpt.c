/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Clear given exceptions in current floating-point environment. */

#include <fenv.h>
#include <shlib-compat.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__feclearexcept (int excepts)
{
  unsigned int pfpfr, fpsr;

  __fenv_getpfpfr (pfpfr);
  __fenv_getfpsr (fpsr);

  /* Mask out unsupported bits/exceptions.  */
  excepts &= FE_ALL_EXCEPT;

  /* Set to 0 selected bits.  */
  __fenv_setpfpfr (pfpfr & ~excepts);
  __fenv_setfpsr (fpsr & ~excepts);

  return 0;
}

libm_hidden_ver (__feclearexcept, feclearexcept)
versioned_symbol (libm, __feclearexcept, feclearexcept, GLIBC_2_2);
