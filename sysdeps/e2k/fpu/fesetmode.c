/* Copyright (c) 2018 ZAO "MCST". All rights reserved. */

/* Install given floating-point control modes. */

#include <fenv.h>
#include <fpu_control.h>

/* All exceptions, including the x86/e2k-specific "denormal operand"
   exception.  */
#define FE_ALL_EXCEPT_X86 (FE_ALL_EXCEPT | __FE_DENORM)

int
fesetmode (const femode_t *modep)
{
  unsigned int pfpfr, fpcr;

  __fenv_getpfpfr (pfpfr);

  pfpfr &= FE_ALL_EXCEPT_X86;

  if (modep == FE_DFL_MODE)
    {
      fpcr = _FPU_FPCR_DEFAULT;
      pfpfr |= FE_ALL_EXCEPT_X86 << 7;
    }
  else
    {
      fpcr = modep->__fpcr;
      pfpfr |= modep->__pfpfr & ~FE_ALL_EXCEPT_X86;
    }
  __fenv_setpfpfr (pfpfr);
  __fenv_setfpcr (fpcr);

  return 0;
}
