/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Install given floating-point environment and raise exceptions. */

#include <fenv.h>
#include <shlib-compat.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__feupdateenv (const fenv_t *envp)
{
  unsigned int pfpfr, fpsr;

  /* Save current exceptions.  */
  __fenv_getpfpfr (pfpfr);
  __fenv_getfpsr (fpsr);
  pfpfr = (pfpfr | fpsr) & FE_ALL_EXCEPT;

  /* Install new environment.  */
  __fesetenv (envp);

  /* Raise the safed exception.  Incidently for us the implementation
     defined format of the values in objects of type fexcept_t is the
     same as the ones specified using the FE_* constants.  */
  __feraiseexcept (pfpfr);

  /* Success.  */
  return 0;
}

/* Stupidly borrowed from `sysdeps/sparc/fpu/feupdateenv.c' to make it possible
   to compile this module with e2k-linux-gcc.  */
libm_hidden_def (__feupdateenv)
libm_hidden_ver (__feupdateenv, feupdateenv)
versioned_symbol (libm, __feupdateenv, feupdateenv, GLIBC_2_2);
