/* Linux pause syscall implementation.
   Copyright (C) 2017-2022 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <https://www.gnu.org/licenses/>.  */

#include <signal.h>
#include <unistd.h>
#include <sysdep-cancel.h>

/* Suspend the process until a signal arrives.
   This always returns -1 and sets errno to EINTR.  */
int
__libc_pause (void)
{
#ifdef __NR_pause
  return SYSCALL_CANCEL (pause);
#else
  return SYSCALL_CANCEL (ppoll, NULL, 0, NULL, NULL);
#endif
}

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__libc_pause, pause)

#else /* defined __e2k__ && defined SHARED  */

#include <shlib-compat.h>

/* Both pause@GLIBC_2.{0,2} in libpthread-2.29.so and libc.so respectively
   are WEAK.  */
weak_alias (__libc_pause, __pause_weak)
versioned_symbol (libc, __pause_weak, pause, GLIBC_2_0);
compat_symbol (libpthread, __pause_weak, pause, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
