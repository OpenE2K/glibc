/* Copyright (C) 1995-2022 Free Software Foundation, Inc.
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

#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sysdep-cancel.h>

/* Wait for pending output to be written on FD.  */
int
__libc_tcdrain (int fd)
{
  /* With an argument of 1, TCSBRK for output to be drain.  */
  return SYSCALL_CANCEL (ioctl, fd, TCSBRK, 1);
}

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__libc_tcdrain, tcdrain)

#else /* defined __e2k__ && defined SHARED  */

#include <shlib-compat.h>

/* Both tcdrain@@GLIBC_2.{0,2} respectively in libpthread-2.29.so and up-to-
   date libc.so are WEAK.  */
weak_alias (__libc_tcdrain, __libc_tcdrain_weak)
versioned_symbol (libc, __libc_tcdrain_weak, tcdrain, GLIBC_2_0);
compat_symbol (libpthread, __libc_tcdrain_weak, tcdrain, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
