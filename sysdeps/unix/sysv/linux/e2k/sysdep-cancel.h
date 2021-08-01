/* Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ilya Malakhov <malakhov@mcst.ru>, 2010.

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

#include <sysdep.h>
#include <tls.h>
#ifndef __ASSEMBLER__
# include <nptl/pthreadP.h>
#endif

#if IS_IN (libc) || IS_IN (libpthread) || IS_IN (librt)

# define __SET_SYSCALL_NAME(syscall_name)                               \
  adds    0x0, SYS_ify(syscall_name), %db[0]

# undef __PSEUDO
# define __PSEUDO(name, syscall_name, args, args_ellipse)               \
  .ignore ld_st_style ;                                                 \
  .type	__##syscall_name##_nocancel,@function;                          \
  .globl	__##syscall_name##_nocancel;                            \
__##syscall_name##_nocancel:                                            \
  setwd   wsz = 0x9 ;                                                   \
  setbn   rsz = 0x3, rbs = 0x5, rcur = 0x0 ;                            \
  __SYSCALL_PROLOGUE ;                                                  \
  __SYSCALL_INPUT_FOR_##args##_##args_ellipse                           \
  sdisp   %ctpr1, __SYSCALL_TRAPNUM;                                    \
  __SET_SYSCALL_NAME (syscall_name);                                    \
  call    %ctpr1, wbs = 0x5;                                            \
  __SYSCALL_OUTPUT                                                      \
  return %ctpr3;                                                        \
  ct %ctpr3;                                                            \
  .size	__##syscall_name##_nocancel,.-__##syscall_name##_nocancel;      \
  ENTRY(name)                                                           \
  setwd   wsz = 0x9 ;                                                   \
  setbn   rsz = 0x3, rbs = 0x5, rcur = 0x0 ;                            \
  __SYSCALL_PROLOGUE ;                                                  \
  __LDW __TLS_REGISTER, MULTIPLE_THREADS_OFFSET, %r8;			\
  CENABLE                                                               \
  __SYSCALL_INPUT_FOR_##args##_##args_ellipse                           \
  sdisp   %ctpr1, __SYSCALL_TRAPNUM;                                    \
  __SET_SYSCALL_NAME (syscall_name);                                    \
  call    %ctpr1, wbs = 0x5;                                            \
  CDISABLE                                                              \
  __SYSCALL_OUTPUT


# define CENABLE_GEN(fn)                                                \
  cmpesb %r8, 0x0, %pred0;                                              \
  disp %ctpr1, fn;                                                      \
  disp %ctpr2, 1f;                                                      \
  ct %ctpr2 ? %pred0;                                                   \
  call %ctpr1, wbs = 0x5;                                               \
  adds 0x0, %b[0], %r9;                                                 \
1:

# define CDISABLE_GEN(fn)                                               \
  cmpesb %r8, 0x0, %pred0;                                              \
  disp %ctpr1, fn;                                                      \
  disp %ctpr2, 1f;                                                      \
  {                                                                     \
  adds 0x0, %r9, %b[0] ? ~%pred0;                                       \
  addd 0x0, %db[0], %dr9 ? ~%pred0;                                     \
  ct %ctpr2 ? %pred0;                                                   \
  };                                                                    \
  call %ctpr1, wbs = 0x5;                                               \
  addd 0x0, %dr9, %db[0];                                               \
1:


# if IS_IN (libpthread)
#  define CENABLE       CENABLE_GEN(__pthread_enable_asynccancel)
#  define CDISABLE      CDISABLE_GEN(__pthread_disable_asynccancel)
#  define enable_asynccancel __pthread_enable_asynccancel
#  define disable_asynccancel __pthread_disable_asynccancel
# elif IS_IN (libc)
#  define CENABLE	CENABLE_GEN(__libc_enable_asynccancel)
#  define CDISABLE	CDISABLE_GEN(__libc_disable_asynccancel)
#  define enable_asynccancel __libc_enable_asynccancel
#  define disable_asynccancel __libc_disable_asynccancel
# elif IS_IN (librt)
#  define CENABLE	CENABLE_GEN(__librt_enable_asynccancel)
#  define CDISABLE	CDISABLE_GEN(__librt_disable_asynccancel)
#  define enable_asynccancel __librt_enable_asynccancel
#  define disable_asynccancel __librt_disable_asynccancel
# else
#  error Unsupported library
# endif


#ifndef __ASSEMBLER__

#  define SINGLE_THREAD_P						\
  __builtin_expect (THREAD_GETMEM (THREAD_SELF,				\
				   header.multiple_threads) == 0, 1)
# else

/* Fortunately we do not need it right now. */
#  define SINGLE_THREAD_P

# endif


#elif /* ! IS_IN (libc) && ! IS_IN (libpthread) && ! IS_IN (librt) && */ !defined __ASSEMBLER__

# define SINGLE_THREAD_P (1)

#endif

#ifndef __ASSEMBLER__
# define RTLD_SINGLE_THREAD_P \
  __builtin_expect (THREAD_GETMEM (THREAD_SELF, \
				   header.multiple_threads) == 0, 1)
#endif
