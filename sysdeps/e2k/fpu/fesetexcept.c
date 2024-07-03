/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

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
