/* POSIX.1 sigaction call for Linux/SPARC.
   Copyright (C) 1997-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Miguel de Icaza <miguel@nuclecu.unam.mx>, 1997.

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

#include <string.h>
#include <syscall.h>
#include <sys/signal.h>
#include <errno.h>
#include <kernel_sigaction.h>
#include <sysdep.h>

#ifndef __LCC__
static void __rt_sigreturn_stub (void);
static void __sigreturn_stub (void);
#else /* __LCC__ */
extern void __rt_sigreturn_stub (void) __attribute__ ((visibility ("hidden")));
extern void __sigreturn_stub (void) __attribute__ ((visibility ("hidden")));
#endif /* __LCC__ */

#define STUB(act, sigsetsize) \
  (act) ? ((unsigned long)((act->sa_flags & SA_SIGINFO)	\
			    ? &__rt_sigreturn_stub	\
			    : &__sigreturn_stub) - 8)	\
	: 0,						\
  (sigsetsize)

#include <sysdeps/unix/sysv/linux/sigaction.c>

#ifndef __LCC__

static
inhibit_stack_protector
void
__rt_sigreturn_stub (void)
{
  __asm__ ("mov %0, %%g1\n\t"
	   "ta	0x10\n\t"
	   : /* no outputs */
	   : "i" (__NR_rt_sigreturn));
}

static
inhibit_stack_protector
void
__sigreturn_stub (void)
{
  __asm__ ("mov %0, %%g1\n\t"
	   "ta	0x10\n\t"
	   : /* no outputs */
	   : "i" (__NR_sigreturn));
}

#else /* __LCC__ */

#undef STR
#undef _STR

#define STR(x) _STR(x)
#define _STR(x) #x

__asm__ ("\n\t.section \".text\""
         "\n\t.align 4"
         "\n\t.type __rt_sigreturn_stub, #function"
         "\n\t.hidden __rt_sigreturn_stub"
         "\n\t.local __rt_sigreturn_stub"
         "\n\t__rt_sigreturn_stub:"
         "\n\tmov "STR (__NR_rt_sigreturn)", %g1"
         "\n\tta 0x10"
         "\n\tjmp %o7+8"
	 "\n\tnop"
         "\n\t .size __rt_sigreturn_stub, .-__rt_sigreturn_stub");

__asm__ ("\n\t.section \".text\""
         "\n\t.align 4"
         "\n\t.type __sigreturn_stub, #function"
         "\n\t.hidden __sigreturn_stub"
         "\n\t.local __sigreturn_stub"
         "\n\t__sigreturn_stub:"
         "\n\tmov "STR (__NR_sigreturn)", %g1"
         "\n\tta 0x10"
         "\n\tjmp %o7+8"
	 "\n\tnop"
         "\n\t.size __sigreturn_stub, .-__sigreturn_stub");
#endif /* __LCC__ */
