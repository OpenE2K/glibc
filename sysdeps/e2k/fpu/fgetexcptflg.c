/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Store current representation for exceptions. */

#include <fenv.h>
#include <shlib-compat.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__fegetexceptflag (fexcept_t *flagp, int excepts)
{
  unsigned int pfpfr, fpsr;

  /* Get the current exceptions.  */
  __fenv_getpfpfr (pfpfr);
  __fenv_getfpsr (fpsr);

  *flagp = (pfpfr | fpsr) & (excepts & FE_ALL_EXCEPT);

  /* Success.  */
  return 0;
}

versioned_symbol (libm, __fegetexceptflag, fegetexceptflag, GLIBC_2_2);
