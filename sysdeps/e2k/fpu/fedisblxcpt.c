/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Disable floating-point exceptions.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 2000.

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
fedisableexcept (int excepts)
{
  unsigned int old_exc, fpcr, pfpfr;

  /* Get the current control word.  */
  __fenv_getpfpfr (pfpfr);
  __fenv_getfpcr (fpcr);

  excepts &= FE_ALL_EXCEPT;
  old_exc = (~pfpfr >> 7) & FE_ALL_EXCEPT;

  /* Set the new control word.  */
  __fenv_setpfpfr (pfpfr | (excepts << 7));
  __fenv_setfpcr (fpcr | excepts);

  return old_exc;
}
