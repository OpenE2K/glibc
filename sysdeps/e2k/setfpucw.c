/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Set the FPU control word. */

#include <math.h>
#include <fpu_control.h>
#include <fenv.h>

#define _FPCR_RESERVED 0xF3C0  /* Reserved bits in %fpcr (cw on i386) */

void
__setfpucw (fpu_control_t set)
{
  fpu_control_t pfpfr, fpcr;

  /* Fetch the current control word.  */
  __fenv_getpfpfr (pfpfr);
  __fenv_getfpcr (fpcr);

  /* Preserve the reserved bits, and set the rest as the user
     specified (or the default, if the user gave zero).  */
  pfpfr &= _FPU_RESERVED;
  pfpfr |= set & ~_FPU_RESERVED;

  __fenv_setpfpfr (pfpfr);

  /* move the same fields (RC & exceptions mask) to their place in %fpcr */
  set = ((set & __FE_ROUND_MASK) >> 3) | ((set >> 7) & FE_ALL_EXCEPT);
  fpcr &= _FPCR_RESERVED;
  fpcr |= set & ~_FPCR_RESERVED;

  __fenv_setfpcr (fpcr);
}
