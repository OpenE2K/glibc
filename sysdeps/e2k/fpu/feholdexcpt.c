/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Store current floating-point environment and clear exceptions. */

#include <fenv.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__feholdexcept (fenv_t *envp)
{
  unsigned int fpsr, fpcr, pfpfr;

  __fenv_getpfpfr (pfpfr);
  __fenv_getfpcr (fpcr);
  __fenv_getfpsr (fpsr);

  /* Store the environment.  */
  envp->__pfpfr = pfpfr;
  envp->__fpcr = fpcr;
  envp->__fpsr = fpsr;

  /* Set all exceptions to non-stop and clear all exceptions.  */
  __fenv_setpfpfr ((pfpfr & ~FE_ALL_EXCEPT) | (FE_ALL_EXCEPT << 7));
  __fenv_setfpcr (fpcr | FE_ALL_EXCEPT);
  __fenv_setfpsr (fpsr & ~FE_ALL_EXCEPT);

  return 0;
}

libm_hidden_def (__feholdexcept)
weak_alias (__feholdexcept, feholdexcept)
libm_hidden_weak (feholdexcept)
