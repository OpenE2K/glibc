/* clone system call for Linux/E2k.
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

#include <asm/errno.h>
#include <asm/unistd.h>
#include <sysdep.h>

#define CLONE_VM        0x00000100
#define CLONE_THREAD    0x00010000

/* FIXME */
#undef r0
#undef r1

/* int __clone (int (*fn)(void *arg), void *child_stack, int flags, void *arg,
                pid_t *ptid, struct user_desc *tls, pid_t *ctid) */

	.ignore ld_st_style
	.text
	.align 8
	.global	__clone
	.type	__clone,@function

__clone:
	setwd	wsz = 0x8
	setbn	rsz = 0x3, rbs = 0x4, rcur = 0x0
	
	/* Sanity check arguments */
	disp	%ctpr1, _error
	adds	0x0, EINVAL, %r7
        {
          nop 2
	  __CMPE	%dr0, 0x0, %pred0
        }
        {
          nop 2
	  __CMPE	%dr1, 0x0, %pred1
        }
	ct	%ctpr1 ? %pred0
	ct	%ctpr1 ? %pred1

	/* Do the system call */
	sdisp	%ctpr1, __SYSCALL_TRAPNUM
	adds	0x0, __NR_clone, %b[0]
	__SYSCALL_ARG_REG (%dr2, %db[1])
	__SYSCALL_ARG_REG (%dr1, %db[2])
        __SYSCALL_ARG_REG (%dr4, %db[3])
        __SYSCALL_ARG_REG (%dr6, %db[4])
        __SYSCALL_ARG_REG (%dr5, %db[5])
	call	%ctpr1, wbs = 0x4

	/* Check the result */
	disp	%ctpr1, _error
	subs	0x0, %b[0], %r7
	disp	%ctpr2, __thread_start
        /* FIXME: one should probably ensure that %b[0] belongs to
	   (-125, -1) range, not that it's just less than zero. */
        {
          nop 2
	  cmplsb	%b[0], 0x0, %pred0
        }
        {
          nop 2
	  cmpesb	%b[0], 0x0, %pred1
        }
	ct	%ctpr1 ? %pred0
	ct	%ctpr2 ? %pred1

	/* Return new thread's pid */
	adds	0x0, %b[0], %r0

	return	%ctpr3
	ct	%ctpr3

_error:
	disp	%ctpr1, HIDDEN_JUMPTARGET (__errno_location)
	call	%ctpr1, wbs = 0x4
	__STW	0x0, %db[0], %r7
	adds	0x0, -1, %r0

	return	%ctpr3
	ct	%ctpr3

	.size __clone, .-__clone

	.type __thread_start,@function

__thread_start:
        __GETPL	%dr0, %ctpr1
	getsp	-16, %empty
	__ADDP	0x0, %r3, %b[0]
	call	%ctpr1, wbs = 0x4



	/* Pass the thread function's return value (it's already on `%b[0]')
	   to `_exit ()'.  */
	disp	%ctpr1, HIDDEN_JUMPTARGET (_exit)
	call	%ctpr1, wbs = 0x4

	.size __thread_start, .-__thread_start

libc_hidden_def (__clone)
weak_alias (__clone, clone)
