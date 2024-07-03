/* Manipulate file descriptor.  Linux LFS version.
   Copyright (C) 2018-2022 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.  */

#define fcntl __no_decl_fcntl
#define __fcntl __no_decl___fcntl
#include <fcntl.h>
#undef fcntl
#undef __fcntl
#include <stdarg.h>
#include <errno.h>
#include <sysdep-cancel.h>

#ifndef __NR_fcntl64
# define __NR_fcntl64 __NR_fcntl
#endif

#ifdef __ptr128__
# undef __NR_fcntl64
# define __NR_fcntl64 __NR_fcntl
#endif

#ifndef FCNTL_ADJUST_CMD
# define FCNTL_ADJUST_CMD(__cmd) __cmd
#endif

int
__libc_fcntl64 (int fd, int cmd, ...)
{
  va_list ap;
  void *arg;

  cmd = FCNTL_ADJUST_CMD (cmd);

  va_start (ap, cmd);
  get_arg;
  va_end (ap);

  if (cmd == F_SETLKW || cmd == F_SETLKW64 || cmd == F_OFD_SETLKW)
    return SYSCALL_CANCEL (fcntl64, fd, cmd, arg);

  return __fcntl64_nocancel_adjusted (fd, cmd, arg);
}
libc_hidden_def (__libc_fcntl64)
weak_alias (__libc_fcntl64, __fcntl64)
libc_hidden_weak (__fcntl64)
weak_alias (__libc_fcntl64, fcntl64)
#if __TIMESIZE != 64
weak_alias (__libc_fcntl64, __fcntl_time64)
#endif

#ifdef __OFF_T_MATCHES_OFF64_T
weak_alias (__libc_fcntl64, __libc_fcntl)
# if ! (defined __e2k__ && defined SHARED)
weak_alias (__libc_fcntl64, __fcntl)
# else /* defined __e2k__ && defined SHARED  */
#  include <shlib-compat.h>
weak_alias (__libc_fcntl64, __fcntl_weak)
versioned_symbol (libc, __fcntl_weak, __fcntl, GLIBC_2_0);
compat_symbol (libpthread, __fcntl_weak, __fcntl, GLIBC_2_0);
# endif /* defined __e2k__ && defined SHARED  */

weak_alias (__libc_fcntl64, __GI___fcntl)

# if ! (defined __e2k__ && defined SHARED)
weak_alias (__libc_fcntl64, fcntl)
# else /* defined __e2k__ && defined SHARED  */
/* In 64-bit modes fcntl@@GLIBC_2.2 () is WEAK unlike 32-bit ones where it's
   GLOBAL. Also in 64-bit modes fcntl () was not probably affected by
   GLIBC_2_28 changes as this happened in 32-bit ones.  */
versioned_symbol (libc, __fcntl_weak, fcntl, GLIBC_2_0);
compat_symbol (libpthread, __fcntl_weak, fcntl, GLIBC_2_0);
# endif /* defined __e2k__ && defined SHARED  */
#endif
