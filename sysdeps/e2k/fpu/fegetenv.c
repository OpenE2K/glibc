/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Store current floating-point environment. */

#include <fenv.h>
#include <shlib-compat.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__fegetenv (fenv_t *envp)
{
  __fenv_getpfpfr (envp->__pfpfr);
  __fenv_getfpcr (envp->__fpcr);
  __fenv_getfpsr (envp->__fpsr);

  /* Success.  */
  return 0;
}

/* FIXME: the lines below are stupidly borrowed from `sysdeps/sparc/fpu
   /fegetenv.c' to make this source compilable with e2k-linux-gcc. Why don't I
   meet analogous problems when compiling with LCC?  */
libm_hidden_def (__fegetenv)
libm_hidden_ver (__fegetenv, fegetenv)
versioned_symbol (libm, __fegetenv, fegetenv, GLIBC_2_2);
