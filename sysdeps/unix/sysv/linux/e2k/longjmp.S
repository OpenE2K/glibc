/* Copyright (C) 1997, 1999 Free Software Foundation, Inc.
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

#define SYSCALL_NAME e2k_longjmp2
#include <sysdep.h>

/* FIXME */
#undef r0
#undef r1

#if ! defined __PROTECTED__
# define val %r1
#else
# define val %r4
#endif

#if defined __PROTECTED__
# define SXT sxt,sm 0x2, %r4, %r4
#else
# define SXT
#endif

#if defined __PROTECTED__
# define GETSAP getsap 0xffffffc0, %qb[0]
#else
# define GETSAP
#endif



#undef __SYSCALL_PROLOGUE
#define __SYSCALL_PROLOGUE \
	GETSAP;		   \
	SXT;		   \
        { \
          nop 2; \
	  cmpesb	val, 0x0, %pred0 \
        }; \
	adds	0x0, 0x1, val ? %pred0


! The kernel sets up %cr registers in such a way that the longjmp syscall
! returns to the function which it has been invoked from. However, on return
! from this function one will find himself in context saved in jmp_buf.

#undef __SYSCALL_OUTPUT
#define __SYSCALL_OUTPUT \
	adds	0x0, %b[0], %r0


PSEUDO (__libc_siglongjmp, SYSCALL_NAME, 2)
	ret
PSEUDO_END (__libc_siglongjmp)

strong_alias (__libc_siglongjmp, __libc_longjmp)
weak_alias (__libc_siglongjmp, __longjmp)
weak_alias (__libc_siglongjmp, _longjmp)
weak_alias (__libc_siglongjmp, longjmp)
weak_alias (__libc_siglongjmp, siglongjmp)
