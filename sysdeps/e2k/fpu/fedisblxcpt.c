/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Disable floating-point exceptions. */

#include <fenv.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
fedisableexcept (int excepts)
{
  unsigned int old_exc, fpcr, pfpfr;

  /* Get the current control word.  */
  __fenv_getpfpfr (pfpfr);
  __fenv_getfpcr (fpcr);

  excepts &= FE_ALL_EXCEPT;
  old_exc = (~pfpfr >> 7) & FE_ALL_EXCEPT;

  /* Set the new control word.  */
  __fenv_setpfpfr (pfpfr | (excepts << 7));
  __fenv_setfpcr (fpcr | excepts);

  return old_exc;
}
