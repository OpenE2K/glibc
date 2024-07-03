/* Copyright (c) 2009-2024 AO MCST.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.  */

#if defined __ptr128__ && defined LIBC_NONSHARED
/* Now that in PM `getcontext ()' has been moved to libc_nonshared.a from
   libc.so ensure that `errno' is referenced as `errno' from the below `INLINE
   _VSYSCALL_ONLY' not as `__libc_errno' locally defined in libc.so.  */
# define __libc_errno errno
#endif

#include <signal.h>
#include <sysdep.h>
/* FIXME: a compile error is going to happen if HAVE_GETCONTEXT_VSYSCALL hasn't
   been defined in `sysdep.h' since we actually don't have any alternative for
   fast syscall here. Shouldn't I implement a stub version of getcontext ()
   instead? On the other hand, upstream glibc should never be compiled with
   ancient Kernel headers . . .  */
#ifdef HAVE_GETCONTEXT_VSYSCALL
# define HAVE_VSYSCALL
#endif /* HAVE_GETCONTEXT_VSYSCALL  */
#include <sysdep-vdso.h>



int
__getcontext (ucontext_t *ucp)
{
  asm ("rrd %%cr0.hi, %0" : "=r" (ucp->uc_mcontext.cr0_hi));
  asm ("rrd %%cr1.lo, %0" : "=r" (ucp->uc_mcontext.cr1_lo));
  asm ("rrd %%cr1.hi, %0" : "=r" (ucp->uc_mcontext.cr1_hi));

  return INLINE_VSYSCALL_ONLY (getcontext, 2, ucp, _NSIG / 8);
}

weak_alias (__getcontext, getcontext)
