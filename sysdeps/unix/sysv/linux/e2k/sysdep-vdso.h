/* vDSO definition specific for E2K Linux.
   Copyright (C) 2015 Free Software Foundation, Inc.
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

#ifndef SYSDEP_VDSO_LINUX_H
# define SYSDEP_VDSO_LINUX_H

extern unsigned long __libc_e2k_fast_syscalls;

# ifdef HAVE_VSYSCALL

#undef INLINE_VSYSCALL
#define INLINE_VSYSCALL(name, nr, args...)                              \
  (__libc_e2k_fast_syscalls & (1 << __NR_fast_sys_##name))              \
  ? __INLINE_SYSCALL (VSYSCALL, name, nr, args)                         \
  : __INLINE_SYSCALL (SYSCALL, name, nr, args)


/* This one should be used in situations where a fast syscall doesn't have
   a slow counterpart. `getcontext ()' is an example.  */
#undef INLINE_VSYSCALL_ONLY
#define INLINE_VSYSCALL_ONLY(name, nr, args...)                         \
  (__libc_e2k_fast_syscalls & (1 << __NR_fast_sys_##name))              \
  ? __INLINE_SYSCALL (VSYSCALL, name, nr, args)                         \
  : ({                                                                  \
      __INTERNAL_SYSCALL_RES(,resultvar) = -1;				\
      __set_errno (ENOSYS);                                             \
      resultvar; })

#undef INTERNAL_VSYSCALL_NCS
#define INTERNAL_VSYSCALL_NCS(name, err, nr, args...)                   \
  __INTERNAL_SYSCALL_NCS (SYSCALL, __VSYSCALL_TRAPNUM, name, err, nr, args)

#define INTERNAL_VSYSCALL(name, err, nr, args...) \
  INTERNAL_VSYSCALL_NCS (__NR_fast_sys_##name, err, nr, args)


# else /* HAVE_VSYSCALL  */

#  define INLINE_VSYSCALL(name, nr, args...) \
    INLINE_SYSCALL (name, nr, ##args)
#  define INTERNAL_VSYSCALL(name, err, nr, args...) \
    INTERNAL_SYSCALL (name, err, nr, ##args)

# endif /* HAVE_VSYSCALL  */


#endif /* SYSDEP_VDSO_LINUX_H  */
