/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Install given floating-point environment.
   Copyright (C) 1997, 1999, 2000 Free Software Foundation, Inc.
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
__fesetenv (const fenv_t *envp)
{
  unsigned int pfpfr, fpcr, fpsr;

  if (envp == FE_DFL_ENV)
    { /* �������� ��� ����� ������ �������� � ��� �� ��������� */
      pfpfr = FE_ALL_EXCEPT << 7;
      fpcr = FE_ALL_EXCEPT;
      fpsr = 0;
    }
  else if (envp == FE_NOMASK_ENV)
    { /* �������� ��� ����� ������ �������� � ��������� �� �� ���������������� */
      pfpfr = 0;
      fpcr = 0;
      fpsr = 0;
    }
  else
    { /* ��� ����� �� envp */
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
