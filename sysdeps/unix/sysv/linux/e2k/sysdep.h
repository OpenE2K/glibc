/* Copyright (C) 2000, 2002, 2003, 2004, 2007 Free Software Foundation, Inc.
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

#ifndef _LINUX_E2K_SYSDEP_H
#define _LINUX_E2K_SYSDEP_H 1

#include <sysdeps/unix/sysv/linux/sysdep.h>


#if IS_IN (rtld)
# include <dl-sysdep.h>		/* Defines RTLD_PRIVATE_ERRNO.  */
#endif

#ifndef __ASSEMBLER__
# define __p1(x) #x
# define __p2(x) __p1(x)
#endif /* __ASSEMBLER__  */

#include <tls.h>

#ifdef __ASSEMBLER__

#if ! defined __SYSCALL_ARG_REG_1
# define __SYSCALL_ARG_REG_1 __SYSCALL_ARG_REG
#endif

#if ! defined __SYSCALL_ARG_REG_2
# define __SYSCALL_ARG_REG_2 __SYSCALL_ARG_REG
#endif

#if ! defined __SYSCALL_ARG_REG_3
# define __SYSCALL_ARG_REG_3 __SYSCALL_ARG_REG
#endif

#if ! defined __SYSCALL_ARG_REG_4
# define __SYSCALL_ARG_REG_4 __SYSCALL_ARG_REG
#endif

#if ! defined __SYSCALL_ARG_REG_5
# define __SYSCALL_ARG_REG_5 __SYSCALL_ARG_REG
#endif

#if ! defined __SYSCALL_ARG_REG_6
# define __SYSCALL_ARG_REG_6 __SYSCALL_ARG_REG
#endif

#define __SYSCALL_PROLOGUE

#define __SYSCALL_INPUT_FOR_0_0

#define __SYSCALL_INPUT_FOR_1_0 \
    __SYSCALL_ARG_REG_1 (%dr0, %db[1]);

#define __SYSCALL_INPUT_FOR_2_0 \
    __SYSCALL_INPUT_FOR_1_0             \
    __SYSCALL_ARG_REG_2 (%dr1, %db[2]);

#define __SYSCALL_INPUT_FOR_3_0 \
    __SYSCALL_INPUT_FOR_2_0             \
    __SYSCALL_ARG_REG_3 (%dr2, %db[3]);

#define __SYSCALL_INPUT_FOR_3_2 \
    getsp 0x0, %dr7 ; \
    __SYSCALL_ARG_REG (%dr0, %db[1]) ; \
    __SYSCALL_ARG_MEM (%dr7, 0x08, %db[2]) ; \
    __SYSCALL_ARG_MEM (%dr7, 0x10, %db[3]) ;

#define __SYSCALL_INPUT_FOR_4_0 \
    __SYSCALL_INPUT_FOR_3_0             \
    __SYSCALL_ARG_REG_4 (%dr3, %db[4]);

#define __SYSCALL_INPUT_FOR_5_0 \
    __SYSCALL_INPUT_FOR_4_0             \
    __SYSCALL_ARG_REG_5 (%dr4, %db[5]);

#define __SYSCALL_INPUT_FOR_6_0 \
    __SYSCALL_INPUT_FOR_5_0             \
    __SYSCALL_ARG_REG_6 (%dr5, %db[6]);


#define __SYS_CALL_ERROR_RETURN_VALUE -1

#ifdef __ptr128_new_abi__
# define WSZ 0x10
# define WBS 0x8
#else /* ! defined __ptr128_new_abi__  */
# define WSZ 0x9
# define WBS 0x5
#endif /* ! defined __ptr128_new_abi__  */

#define __SET_ERRNO \
    adds   0x0, %b[0], %r7 ? %pred0 ; \
    disp   %ctpr1, __errno_location ; \
    call   %ctpr1, wbs = WBS ? %pred0 ; \
    __STW  %b[0], 0x0, %r7 ? %pred0


#define __SYSCALL_OUTPUT \
    cmpldb  %db[0], 0, %pred0 ; \
    addd    %db[0], 0, %dr0 ; \
    addd    0x0, __SYS_CALL_ERROR_RETURN_VALUE, %dr0 ? %pred0 ; \
    subd    0x0, %db[0], %db[0] ? %pred0 ; \
    __SET_ERRNO ;

#undef SYS_ify
#define SYS_ify(syscall_name)  __NR_##syscall_name

/* This may be overridden by the implementation relying on new PM
   syscalls.  */
#ifndef __SET_SYSCALL_NAME
# define __SET_SYSCALL_NAME(syscall_name)	\
  adds    0x0, SYS_ify(syscall_name), %db[0]
#endif


#define __PSEUDO(name, syscall_name, args, args_ellipse) \
        .ignore ld_st_style ; \
ENTRY(name) \
        setwd   wsz = WSZ ; \
        setbn   rsz = WSZ - WBS - 1, rbs = WBS, rcur = 0x0 ; \
        __SYSCALL_PROLOGUE ; \
        __SYSCALL_INPUT_FOR_##args##_##args_ellipse  \
        sdisp   %ctpr1, __SYSCALL_TRAPNUM ; \
        __SET_SYSCALL_NAME(syscall_name); \
        call    %ctpr1, wbs = WBS ; \
        __SYSCALL_OUTPUT

#define PSEUDO(name, syscall_name, args)  __PSEUDO(name, syscall_name, args, 0)

#define PSEUDO_NOERRNO(name, syscall_name, args) \
        .ignore ld_st_style ; \
ENTRY(name) \
        setwd   wsz = WSZ ; \
        setbn   rsz = WSZ - WBS - 1, rbs = WBS, rcur = 0x0 ; \
        __SYSCALL_PROLOGUE ; \
        __SYSCALL_INPUT_FOR_##args##_0  \
        sdisp   %ctpr1, __SYSCALL_TRAPNUM ; \
        __SET_SYSCALL_NAME(syscall_name); \
        call    %ctpr1, wbs = WBS ; \
        __SYSCALL_OUTPUT

#else /* not __ASSEMBLER__ */

#define SPIN_LOCK_UNLOCKED_VAL	1
#define SPIN_LOCK_LOCKED_VAL	0

#undef INTERNAL_SYSCALL_DECL
#define INTERNAL_SYSCALL_DECL(err) do { } while (0)

#if ! defined INTERNAL_SYSCALL_ERROR_P
#define INTERNAL_SYSCALL_ERROR_P(val, err) \
  ((unsigned long ) (val) >= -125UL)
#endif

#undef INTERNAL_SYSCALL_ERRNO
/* An explicit cast to `long' is required here because of Protected Mode where
   system call value (VAL) is currently of type `void *'.  */
#define INTERNAL_SYSCALL_ERRNO(val, err)    (- (long) (val))


#define __INTERNAL_SYSCALL_CLOBBERS                   \
  "call", "memory"


#if ! defined __INTERNAL_SYSCALL_NCS
/* This macro is used both in `INTERNAL_{,V}SYSCALL_NCS' implementations.
   Probably it should be named in a more appropriate way.  */
#define __INTERNAL_SYSCALL_NCS(sort, trapnum, name, err, nr, args...)	\
  ({                                                                    \
    __INTERNAL_SYSCALL_RES (sort, syscall_res);				\
    __INTERNAL_SYSCALL_ERR (sort, __err);                               \
    __asm__ __volatile__                                                \
      (__INTERNAL_SYSCALL_SDISP (trapnum)				\
       __INTERNAL_SYSCALL_LOAD_ARGS_##nr                                \
       __INTERNAL_SYSCALL_CALL                                          \
       __INTERNAL_SYSCALL_LOAD_RES (sort)				\
       : __INTERNAL_SYSCALL_OUT_CONSTRAINTS (sort, syscall_res)		\
       : __INTERNAL_SYSCALL_IN_CONSTRAINTS_##nr (name, args)            \
       : __INTERNAL_SYSCALL_CLOBBERS);                                  \
    __INTERNAL_SYSCALL_RETURN_RES (sort, syscall_res);			\
  })

#endif /* ! defined __INTERNAL_SYSCALL_NCS  */

#define __INLINE_SYSCALL(sort, name, nr, args...)        \
  ({                                          \
    __INTERNAL_SYSCALL_RES(sort, resultvar) = INTERNAL_##sort (name, , nr, args); \
    if (__builtin_expect (INTERNAL_SYSCALL_ERROR_P (resultvar, ), 0))         \
      {                                       \
        __set_errno (INTERNAL_SYSCALL_ERRNO (resultvar, ));     \
        resultvar = (typeof (resultvar)) -1;			\
      }                                                         \
    resultvar; })



#undef INLINE_SYSCALL
#define INLINE_SYSCALL(name, nr, args...)       \
  __INLINE_SYSCALL (SYSCALL, name, nr, args)

#undef INTERNAL_SYSCALL_NCS
#define INTERNAL_SYSCALL_NCS(name, err, nr, args...)    \
  __INTERNAL_SYSCALL_NCS (SYSCALL, __SYSCALL_TRAPNUM, name, err, nr, args)

#ifndef INTERNAL_SYSCALL
# define INTERNAL_SYSCALL(name, err, nr, args...)	\
  INTERNAL_SYSCALL_NCS (__NR_##name, err, nr, args)
#endif


extern unsigned long __libc_e2k_system_info;

#define __INTERNAL_SYSCALL_SDISP(trapnum)	\
  "{\n\t"                                       \
  "  sdisp  %%ctpr1, " __p2(trapnum) "\n\t"     \
  "}\n\t"

#define __INTERNAL_SYSCALL_CALL        \
    "call   %%ctpr1, wbs = %#\n\t"

#ifndef __INTERNAL_SYSCALL_OUT_CONSTRAINTS
#define __INTERNAL_SYSCALL_OUT_CONSTRAINTS(sort, r)	\
  [res] "=r" (r)
#endif

#ifndef __INTERNAL_SYSCALL_RETURN_ERR
# define __INTERNAL_SYSCALL_RETURN_ERR(sort, e)
#endif

#ifndef __INTERNAL_SYSCALL_ERR
# define __INTERNAL_SYSCALL_ERR(sort, e)
#endif


#ifndef __INTERNAL_SYSCALL_RETURN_RES
# define __INTERNAL_SYSCALL_RETURN_RES(sort, r) \
  (r)
#endif


#if ! defined __INTERNAL_SYSCALL_IN_CONSTRAINTS_0

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_0(name, nonexistent)  \
  [num] "ri" (name)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_1(name, a1)           \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_0 (name,),                  \
    [arg1] "ri" ((long) (a1))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_2(name, a1, a2)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_1 (name, a1),               \
    [arg2] "ri" ((long) (a2))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_3(name, a1, a2, a3)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_2 (name, a1, a2),           \
    [arg3] "ri" ((long) (a3))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_4(name, a1, a2, a3, a4)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_3 (name, a1, a2, a3),               \
    [arg4] "ri" ((long) (a4))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_5(name, a1, a2, a3, a4, a5)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_4 (name, a1, a2, a3, a4),           \
    [arg5] "ri" ((long) (a5))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_6(name, a1, a2, a3, a4, a5, a6) \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_5 (name, a1, a2, a3, a4, a5),       \
    [arg6] "ri" ((long) (a6))

#endif /* ! defined __INTERNAL_SYSCALL_IN_CONSTRAINTS_0  */

/* List of system calls which are supported as vsyscalls
   (fast syscalls).  */
#ifdef __NR_fast_sys_gettimeofday
# define HAVE_GETTIMEOFDAY_VSYSCALL	1
#endif /* __NR_fast_sys_gettimeofday  */

#ifdef __NR_fast_sys_clock_gettime
# define HAVE_CLOCK_GETTIME_VSYSCALL	1
#endif /* __NR_fast_sys_clock_gettime  */

#ifdef __NR_fast_sys_getcpu
# define HAVE_GETCPU_VSYSCALL		1
#endif /* __NR_fast_sys_getcpu  */

#ifdef __NR_fast_sys_siggetmask
# define HAVE_SIGGETMASK_VSYSCALL	1
#endif /* __NR_fast_sys_siggetmask  */

#ifdef __NR_fast_sys_getcontext
# define HAVE_GETCONTEXT_VSYSCALL	1
#endif /* __NR_fast_sys_getcontext  */


#endif /* __ASSEMBLER__ */

/* Pointer mangling support.  */
#if ! IS_IN (libc) && IS_IN (rtld)
/* We cannot use the thread descriptor because in ld.so we use setjmp
   earlier than the descriptor is initialized.  */
#else /* IS_IN (libc) || ! IS_IN (rtld)  */
# ifdef __ASSEMBLER__
#  define PTR_MANGLE(dreg, reg, tmpreg) \
  __LDW	__TLS_REGISTER, POINTER_GUARD, tmpreg; \
  xor	reg, tmpreg, dreg
#  define PTR_DEMANGLE(dreg, reg, tmpreg) PTR_MANGLE (dreg, reg, tmpreg)
#  define PTR_MANGLE2(dreg, reg, tmpreg) \
  xor	reg, tmpreg, dreg
#  define PTR_DEMANGLE2(dreg, reg, tmpreg) PTR_MANGLE2 (dreg, reg, tmpreg)
# else /* __ASSEMBLER__ */

/* Avoid mangling pointers to functions this way in Protected Mode since this
   will obviously invalidate their external tags and therefore make them
   unusable.  */
#  if ! defined __ptr128__
#   define PTR_MANGLE(var) \
  (var) = (__typeof (var)) ((uintptr_t) (var) ^ THREAD_GET_POINTER_GUARD ())
#   define PTR_DEMANGLE(var)     PTR_MANGLE (var)
#  endif /* ! defined __ptr128__  */

# endif /* ! __ASSEMBLER__  */
#endif /* IS_IN (libc) || ! IS_IN (rtld)  */

#if defined __LCC__
/* This attribute is used to mark functions which shouldn't be
   inlined when compiling with `-fwhole' option (see Bug #61026).
   I wonder if this is going to protect us from inlining their
   numerous aliases (versioned symbols and so on . . .). Should
   they be explicitly marked with this attribute or not? This
   depends on our `-fwhole' implementation which nobody is aware
   of.  */
# define ATTRIBUTE_NOINLINE_WHOLE __attribute__ ((noinline))
#else /* __LCC__  */
# define ATTRIBUTE_NOINLINE_WHOLE
#endif /* __LCC__  */


#endif /* _LINUX_E2K_SYSDEP_H */
