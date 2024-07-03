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

#ifndef _LINUX_E2K64_SYSDEP_H
#define _LINUX_E2K64_SYSDEP_H 1


#include <sysdeps/unix/e2k/sysdep.h>

#define __LDD   ldd
#define __STD   std
#define __LDW   ldw
#define __STW   stw
#define __LDP   ldd
#define __STP   std
#define __ADDP  addd
#define __GETPL movtd
#define __CMPE  cmpedb
#define __MERGE merged
#define __MOV64 addd 0x0,

#define __SYSCALL_TRAPNUM 3
#define __VSYSCALL_TRAPNUM 6

#ifdef __ASSEMBLER__

#define __SYSCALL_ARG_REG(src, dst)		addd,sm 0x0, src, dst
#define __SYSCALL_ARG_MEM(src1, src2, dst)	ldd src1, src2, dst

#else /* not __ASSEMBLER__ */

#define __p1(x) #x
#define __p2(x) __p1(x)

# include <errno.h>

#define __INTERNAL_SYSCALL_RES(sort, var)	\
  register long var


#define __INTERNAL_SYSCALL_LOAD_ARGS_0          \
  "addd 0x0, %[num], %%b[0]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_1          \
  __INTERNAL_SYSCALL_LOAD_ARGS_0                \
  "addd 0x0, %[arg1], %%b[1]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_2          \
  __INTERNAL_SYSCALL_LOAD_ARGS_1                \
  "addd 0x0, %[arg2], %%b[2]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_3          \
  __INTERNAL_SYSCALL_LOAD_ARGS_2                \
  "addd 0x0, %[arg3], %%b[3]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_4          \
  __INTERNAL_SYSCALL_LOAD_ARGS_3                \
  "addd 0x0, %[arg4], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_5          \
  __INTERNAL_SYSCALL_LOAD_ARGS_4                \
  "addd 0x0, %[arg5], %%b[5]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_6          \
  __INTERNAL_SYSCALL_LOAD_ARGS_5                \
  "addd 0x0, %[arg6], %%b[6]\n\t"


#define __INTERNAL_SYSCALL_LOAD_RES(sort)	\
  "addd 0x0, %%db[0], %[res]\n\t"

#endif /* __ASSEMBLER__ */

#include <sysdeps/unix/sysv/linux/e2k/sysdep.h>


  /* We used to have illegal definitions for `F_{GETLK,SETLK,SETLKW}64'
     in 64-bit mode in `sysdeps/unix/sysv/linux/e2k/bits/fcntl.h' (see
     Bug #45495). This check lets us support "ancient" dynamic
     executables compiled with invalid headers.   */
#define FCNTL_ADJUST_CMD(__cmd)                 \
  ({ int cmd_ = (__cmd);                        \
    if (cmd_ >= 12 && cmd_ <= 14)               \
      cmd_ -= 12 - F_GETLK;                     \
    cmd_;})

#endif /* _LINUX_E2K64_SYSDEP_H */
