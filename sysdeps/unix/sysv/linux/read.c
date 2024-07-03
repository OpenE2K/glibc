/* Linux read syscall implementation.
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

/* Read NBYTES into BUF from FD.  Return the number read or -1.  */
ssize_t
__libc_read (int fd, void *buf, size_t nbytes)
{
  return SYSCALL_CANCEL (read, fd, buf, nbytes);
}
libc_hidden_def (__libc_read)

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (__read)

#else /* defined __e2k__ && defined SHARED  */

# include <shlib-compat.h>

/* Note that if __read was created by the inhibited libc_hidden_def (...) it
   would be non-WEAK. Unlike it compat __read@GLIBC_2.0 should be WEAK.  */
versioned_symbol (libc, __libc_read, __read, GLIBC_2_0);
weak_alias (__libc_read, __libc_read_weak)
compat_symbol (libpthread, __libc_read_weak, __read, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */

/* No harm in e2k SHARED case as this eventually results in
   `__GI___read = __GI___libc_read'. However, it's not quite clear
   if __GI___read is of any purpose with the preceding libc_hidden_def ()
   disabled.  */
weak_alias (__libc_read, __read)

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (read)

#else /* defined __e2k__ && defined SHARED  */

/* Note that if read was created by the inhibited libc_hidden_def (...) it
   would be non-WEAK. Unlike it compat read@GLIBC_2.0 should be WEAK.  */
versioned_symbol (libc, __libc_read, read, GLIBC_2_0);
compat_symbol (libpthread, __libc_read_weak, read, GLIBC_2_0);


#endif /* defined __e2k__ && defined SHARED  */


/* No harm in e2k SHARED case as this eventually results in
   `__GI_read = __GI___libc_read'. However, it's not quite clear
   if __GI_read is of any purpose with the preceding libc_hidden_def ()
   disabled.  */
weak_alias (__libc_read, read)
