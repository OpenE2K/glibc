/* Copyright (c) 2018 ZAO "MCST". All rights reserved. */

/* Store current floating-point control modes. */

#include <fenv.h>
#include <fpu_control.h>

int
fegetmode (femode_t *modep)
{
  __fenv_getpfpfr (modep->__pfpfr);
  __fenv_getfpcr (modep->__fpcr);

  return 0;
}
