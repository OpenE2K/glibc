/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Get enabled floating-point exceptions. */

#include <fenv.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
fegetexcept (void)
{
  unsigned int pfpfr;

  /* Get the current control word.  */
  __fenv_getpfpfr (pfpfr);

  return (~pfpfr >> 7) & FE_ALL_EXCEPT;
}
