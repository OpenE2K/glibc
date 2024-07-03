/* Synchronize a file's in-core state with storage device Linux
   implementation.
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

/* Make all changes done to FD actually appear on disk.  */
int
#if ! (defined __e2k__ && defined SHARED)
fsync
#else /* defined __e2k__ && defined SHARED  */
/* fsync isn't subject to libc_hidden_{proto & def} magic and thus would be
   output "as is". Rename so as to make it possible to create names with
   explicitly specified versions (i.e. `fsync@*'s) below.  */
__fsync_strong
#endif /* defined __e2k__ && defined SHARED  */
(int fd)
{
  return SYSCALL_CANCEL (fsync, fd);
}

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (fsync)

#else /* defined __e2k__ && defined SHARED  */

#include <shlib-compat.h>

/* This creates __GI_fsync thanks to `libc_hidden_proto ()'s magic in one of
   the included headers. In the original code this symbol is assigned to the
   body of fsync () via the same magic.  */
strong_alias(__fsync_strong, fsync)

/* This stands for libc_hidden_def () in the original code: because we need two
   versions for fsync (), both of them need to be explicitly specified.
   Both `fsync@GLIBC_2.{0,2}'s are NOT WEAK.  */
versioned_symbol (libc, __fsync_strong, fsync, GLIBC_2_0);
compat_symbol (libpthread, __fsync_strong, fsync, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
