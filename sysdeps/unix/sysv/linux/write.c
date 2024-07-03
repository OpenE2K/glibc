/* Linux write syscall implementation.
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
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <unistd.h>
#include <sysdep-cancel.h>

/* Write NBYTES of BUF to FD.  Return the number written, or -1.  */
ssize_t
__libc_write (int fd, const void *buf, size_t nbytes)
{
  return SYSCALL_CANCEL (write, fd, buf, nbytes);
}
libc_hidden_def (__libc_write)

weak_alias (__libc_write, __write)
#if ! (defined __e2k__ && defined SHARED)
libc_hidden_weak (__write)
#endif /* ! (defined __e2k__ && defined SHARED)  */

weak_alias (__libc_write, write)
#if ! (defined __e2k__ && defined SHARED)
libc_hidden_weak (write)
#endif /* ! (defined __e2k__ && defined SHARED)  */

#if defined __e2k__ && defined SHARED

#include <shlib-compat.h>

/* Both {__,}write are WEAK. This is true both for their default and compat
   versions.  */
weak_alias (__libc_write, __write_weak)
versioned_symbol (libc, __write_weak, __write, GLIBC_2_2);
versioned_symbol (libc, __write_weak, write, GLIBC_2_2);

compat_symbol (libpthread, __write_weak, __write, GLIBC_2_0);
compat_symbol (libpthread, __write_weak, write, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
