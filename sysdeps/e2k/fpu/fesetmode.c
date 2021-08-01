/* Copyright (c) 2018 ZAO "MCST". All rights reserved. */

/* Install given floating-point control modes.
   Copyright (C) 2016-2018 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <fenv.h>
#include <fpu_control.h>

/* All exceptions, including the x86/e2k-specific "denormal operand"
   exception.  */
#define FE_ALL_EXCEPT_X86 (FE_ALL_EXCEPT | __FE_DENORM)

int
fesetmode (const femode_t *modep)
{
  unsigned int pfpfr, fpcr;

  __fenv_getpfpfr (pfpfr);

  pfpfr &= FE_ALL_EXCEPT_X86;

  if (modep == FE_DFL_MODE)
    {
      fpcr = _FPU_FPCR_DEFAULT;
      pfpfr |= FE_ALL_EXCEPT_X86 << 7;
    }
  else
    {
      fpcr = modep->__fpcr;
      pfpfr |= modep->__pfpfr & ~FE_ALL_EXCEPT_X86;
    }
  __fenv_setpfpfr (pfpfr);
  __fenv_setfpcr (fpcr);

  return 0;
}
