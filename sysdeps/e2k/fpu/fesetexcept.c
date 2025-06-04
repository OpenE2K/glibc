/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Set given exception flags. */

#include <fenv.h>

int
fesetexcept (int excepts)
{
  unsigned int pfpfr, fpsr;

  __fenv_getpfpfr (pfpfr);
  __fenv_getfpsr (fpsr);

  excepts &= FE_ALL_EXCEPT;

  __fenv_setpfpfr (pfpfr | excepts);
  __fenv_setfpsr (fpsr | excepts);

  return 0;
}
