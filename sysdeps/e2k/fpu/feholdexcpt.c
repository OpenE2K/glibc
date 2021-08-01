/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Store current floating-point environment and clear exceptions.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

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
