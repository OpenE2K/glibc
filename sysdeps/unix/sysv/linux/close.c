/* Linux close syscall implementation.
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
#include <not-cancel.h>

/* Close the file descriptor FD.  */
int
#if ! (defined __e2k__ && defined SHARED)
__close
#else /* defined __e2k__ && defined SHARED  */
static
__close_local
#endif /* defined __e2k__ && defined SHARED  */
(int fd)
{
  return SYSCALL_CANCEL (close, fd);
}

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (__close)
strong_alias (__close, __libc_close)
weak_alias (__close, close)

#else /* defined __e2k__ && defined SHARED  */
# include <shlib-compat.h>

/* This alias ensures the creation of __GI_* () HIDDEN symbol (to which
   "aliasname" is transformed via libc_hidden_proto magic) intended for
   internal use within libc.so.  */
strong_alias (__close_local, __close)

strong_alias (__close_local, __close_strong)
versioned_symbol (libc, __close_strong, __close, GLIBC_2_0);
compat_symbol (libpthread, __close_strong, __close, GLIBC_2_0);

strong_alias (__close_local, __libc_close)

weak_alias (__close_strong, __close_weak)
versioned_symbol (libc, __close_weak, close, GLIBC_2_0);
compat_symbol (libpthread, __close_weak, close, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
