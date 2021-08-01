/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Set the FPU control word.
   Copyright (C) 1996, 1997, 1999 Free Software Foundation, Inc.
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

#include <math.h>
#include <fpu_control.h>
#include <fenv.h>

#define _FPCR_RESERVED 0xF3C0  /* Reserved bits in %fpcr (cw on i386) */

void
__setfpucw (fpu_control_t set)
{
  fpu_control_t pfpfr, fpcr;

  /* Fetch the current control word.  */
  __fenv_getpfpfr (pfpfr);
  __fenv_getfpcr (fpcr);

  /* Preserve the reserved bits, and set the rest as the user
     specified (or the default, if the user gave zero).  */
  pfpfr &= _FPU_RESERVED;
  pfpfr |= set & ~_FPU_RESERVED;

  __fenv_setpfpfr (pfpfr);

  /* move the same fields (RC & exceptions mask) to their place in %fpcr */
  set = ((set & __FE_ROUND_MASK) >> 3) | ((set >> 7) & FE_ALL_EXCEPT);
  fpcr &= _FPCR_RESERVED;
  fpcr |= set & ~_FPCR_RESERVED;

  __fenv_setfpcr (fpcr);
}
