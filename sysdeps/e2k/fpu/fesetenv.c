/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Install given floating-point environment. */

#include <fenv.h>
#include <shlib-compat.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__fesetenv (const fenv_t *envp)
{
  unsigned int pfpfr, fpcr, fpsr;

  if (envp == FE_DFL_ENV)
    { /* обнуляем все флаги особых ситуаций и все их маскируем */
      pfpfr = FE_ALL_EXCEPT << 7;
      fpcr = FE_ALL_EXCEPT;
      fpsr = 0;
    }
  else if (envp == FE_NOMASK_ENV)
    { /* обнуляем все флаги особых ситуаций и оставляем их не замаскированными */
      pfpfr = 0;
      fpcr = 0;
      fpsr = 0;
    }
  else
    { /* все берем из envp */
      pfpfr = envp->__pfpfr;
      fpcr = envp->__fpcr;
      fpsr = envp->__fpsr;
    }

  __fenv_setpfpfr (pfpfr);
  __fenv_setfpcr (fpcr);
  __fenv_setfpsr (fpsr);

  /* Success.  */
  return 0;
}

libm_hidden_def (__fesetenv)
libm_hidden_ver (__fesetenv, fesetenv)
versioned_symbol (libm, __fesetenv, fesetenv, GLIBC_2_2);
