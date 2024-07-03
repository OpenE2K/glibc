/* Linux synchronize a file with a memory map implementation.
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

#include <sys/mman.h>
#include <sysdep-cancel.h>

int
#if ! (defined __e2k__ && defined SHARED)
msync
#else /* defined __e2k__ && defined SHARED  */
/* fsync isn't subject to libc_hidden_{proto & def} magic and thus would be
   output "as is". Rename so as to make it possible to create names with
   explicitly specified versions (i.e. `fsync@*'s) below.  */
__msync_strong
#endif /* defined __e2k__ && defined SHARED  */
(void *addr, size_t length, int flags)
{
  return SYSCALL_CANCEL (msync, addr, length, flags);
}

#if defined __e2k__ && defined SHARED

#include <shlib-compat.h>

/* Both `msync@GLIBC_2.{0,2}'s are NOT WEAK.  */
versioned_symbol (libc, __msync_strong, msync, GLIBC_2_0);
compat_symbol (libpthread, __msync_strong, msync, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
