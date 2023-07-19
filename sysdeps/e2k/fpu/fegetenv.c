/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Store current floating-point environment.
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
