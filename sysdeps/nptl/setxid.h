/* Copyright (C) 2004-2019 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#include <nptl/pthreadP.h>
#include <sysdep.h>

#define __SETXID_1(cmd, arg1) \
  cmd.id[0] = (long int) arg1
#define __SETXID_2(cmd, arg1, arg2) \
  __SETXID_1 (cmd, arg1); cmd.id[1] = (long int) arg2
#define __SETXID_3(cmd, arg1, arg2, arg3) \
  __SETXID_2 (cmd, arg1, arg2); cmd.id[2] = (long int) arg3

#ifdef SINGLE_THREAD
# define INLINE_SETXID_SYSCALL(name, nr, args...) \
  INLINE_SYSCALL (name, nr, args)
#elif defined SHARED
# define INLINE_SETXID_SYSCALL(name, nr, args...) \
  ({									\
    int __result;							\
    if (__builtin_expect (__libc_pthread_functions_init, 0))		\
      {									\
	struct xid_command __cmd;					\
	/* FIXME: to be placed under "ifdef __ptr128__" as this is done	\
	   for the sake of PM.  */					\
	__cmd.id[0] = __cmd.id[1] = __cmd.id[2] = 0;			\
	__cmd.syscall_no = __NR_##name;					\
	__SETXID_##nr (__cmd, args);					\
	__result = PTHFCT_CALL (ptr__nptl_setxid, (&__cmd));		\
	}								\
    else								\
      __result = INLINE_SYSCALL (name, nr, args);			\
    __result;								\
   })
#else
# define INLINE_SETXID_SYSCALL(name, nr, args...) \
  ({									\
    extern __typeof (__nptl_setxid) __nptl_setxid __attribute__((weak));\
    int __result;							\
    if (__glibc_unlikely (__nptl_setxid	!= NULL))			      \
      {									\
	struct xid_command __cmd;					\
	/* FIXME: to be placed under "ifdef __ptr128__" as this is done	\
	   for the sake of PM.  */					\
	__cmd.id[0] = __cmd.id[1] = __cmd.id[2] = 0;			\
	__cmd.syscall_no = __NR_##name;					\
	__SETXID_##nr (__cmd, args);					\
	__result =__nptl_setxid (&__cmd);				\
      }									\
    else								\
      __result = INLINE_SYSCALL (name, nr, args);			\
    __result;								\
   })
#endif
