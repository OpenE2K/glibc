/* clone2 system call for Linux/E2k.
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

/* clone2() is even more special than fork() as it mucks with stacks
   and invokes a function in the right context after its all over.  */

#include <asm/errno.h>
#include <asm/unistd.h>
#include <sysdep.h>

/* FIXME */
#undef r0
#undef r1

/* FIXME: this function has been initially implemented without requirements of
   NPTL-specific implementation of `getpid ()' taken into account. The need for
   such compatibility used to be indicated by `#ifdef RESET_PID', RESET_PID used
   to be defined in NPTL-specific version of `clone2.S', which in its turn
   included this file. Now that NPTL is no longer an add-on, all Linux
   distributions, which support it (including E2K), should provide compatibility
   with NPTL implicitly. The only reason why I don't remove the aforementioned
   `#ifdef' is that we've never had it here, unlike `clone ()'. On the other
   hand, is `clone2 ()' at E2K worth this trouble? */

/* int __clone2 (int (*fn)(void *arg), void *child_stack_base,
                 size_t child_stack_size, int flags, void *arg) */

	.ignore ld_st_style
	.text
	.align 8
	.global	__clone2
	.type	__clone2,@function

__clone2:
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
	adds	0x0, __NR_clone2, %b[0]
	__SYSCALL_ARG_REG (%dr3, %db[1])
	__SYSCALL_ARG_REG (%dr1, %db[2])
	__SYSCALL_ARG_REG (%dr2, %db[3])
	call	%ctpr1, wbs = 0x4

	/* Check the result */
	disp	%ctpr1, _error
	subs	0x0, %b[0], %r7
	disp	%ctpr2, __thread_start
/* FIXME ne < 0,  a diapazaon (-125,-1) */
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

	.size __clone2, .-__clone2

	.type __thread_start,@function

__thread_start:
	__GETPL	%dr0, %ctpr1
	getsp	-16, %empty
	__ADDP	0x0, %dr4, %db[0]
	call	%ctpr1, wbs = 0x4

	/* Pass the thread function's return value (it's already on `%b[0]')
	   to `_exit ()'.  */
	disp	%ctpr1, HIDDEN_JUMPTARGET (_exit)
	call	%ctpr1, wbs = 0x4

	.size __thread_start, .-__thread_start

weak_alias (__clone2, clone2)
