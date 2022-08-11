/* Copyright (C) 1997, 2002, 2003, 2004, 2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Miguel de Icaza <miguel@gnu.ai.mit.edu>, January 1997.

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

#ifndef _LINUX_E2K32_SYSDEP_H
#define _LINUX_E2K32_SYSDEP_H 1


#include <sysdeps/unix/e2k/sysdep.h>


#define __LDD   ldgdd
#define __STD   stgdd
#define __LDW   ldgdw
#define __STW   stgdw
#define __LDP   ldgdw
#define __STP   stgdw
#define __ADDP  adds
#define __GETPL getpl
#define __CMPE  cmpesb
#define __MERGE merges
#define __MOV64 sxt 6,

/* For a number of 32-bit syscalls such as . . . we'd still prefer to
   use the old trapnum value in which case `__SYSCALL_TRAPNUM' has
   already been defined (to FOUR).  */
#ifndef __SYSCALL_TRAPNUM

# define __SYSCALL_TRAPNUM 1
# define __MOVE "adds 0x0, "
# define __MOVE_SPEC adds,sm 0x0,

#else

/* When using the old trapnum 4, `sxt 6' should be used rather than `adds 0'
   to put system call parameters into registers, because the Kernel is uncapable
   of clearing the higher 32-bits in this case.  */
# define __MOVE "sxt 0x6, "
# define __MOVE_SPEC sxt,sm 0x6,

#endif

#define __VSYSCALL_TRAPNUM 5

#ifdef __ASSEMBLER__

#define __SYSCALL_ARG_REG(src, dst)		__MOVE_SPEC src, dst
#define __SYSCALL_ARG_MEM(src1, src2, dst)	ldgdw src1, src2, dst

#else /* not __ASSEMBLER__ */

#define __p1(x) #x
#define __p2(x) __p1(x)

#include <errno.h>

#define __INTERNAL_SYSCALL_RES(sort, var)	\
  register long var


#define __INTERNAL_SYSCALL_LOAD_ARGS_0          \
  __MOVE "%[num], %%b[0]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_1          \
  __INTERNAL_SYSCALL_LOAD_ARGS_0                \
  __MOVE "%[arg1], %%b[1]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_2          \
  __INTERNAL_SYSCALL_LOAD_ARGS_1                \
  __MOVE "%[arg2], %%b[2]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_3          \
  __INTERNAL_SYSCALL_LOAD_ARGS_2                \
  __MOVE "%[arg3], %%b[3]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_4          \
  __INTERNAL_SYSCALL_LOAD_ARGS_3                \
  __MOVE "%[arg4], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_5          \
  __INTERNAL_SYSCALL_LOAD_ARGS_4                \
  __MOVE "%[arg5], %%b[5]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_6          \
  __INTERNAL_SYSCALL_LOAD_ARGS_5                \
  __MOVE "%[arg6], %%b[6]\n\t"


#define __INTERNAL_SYSCALL_LOAD_RES(sort)	\
  "adds 0x0, %%db[0], %[res]\n\t"

#endif /* __ASSEMBLER__ */

#include <sysdeps/unix/sysv/linux/e2k/sysdep.h>

#endif /* _LINUX_E2K32_SYSDEP_H */
