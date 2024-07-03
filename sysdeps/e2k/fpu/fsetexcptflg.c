/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Set floating-point environment exception handling. */

#include <fenv.h>
#include <shlib-compat.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__fesetexceptflag (const fexcept_t *flagp, int excepts)
{
  unsigned int pfpfr, fpsr;

  /* Get the current exceptions.  */
  __fenv_getpfpfr (pfpfr);
  __fenv_getfpsr (fpsr);

  excepts &= FE_ALL_EXCEPT;
  pfpfr &= ~excepts;
  fpsr &= ~excepts;

  __fenv_setpfpfr (pfpfr | (*flagp & excepts));
  __fenv_setfpsr (fpsr | (*flagp & excepts));

  /* Success.  */
  return 0;
}

versioned_symbol (libm, __fesetexceptflag, fesetexceptflag, GLIBC_2_2);
