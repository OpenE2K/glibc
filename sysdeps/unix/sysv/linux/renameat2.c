/* Linux implementation for renameat2 function.
   Copyright (C) 2018-2019 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <stdio.h>
#include <sysdep.h>

int
renameat2 (int oldfd, const char *old, int newfd, const char *new,
           unsigned int flags)
{
#if !defined (__NR_renameat) || defined (__ASSUME_RENAMEAT2)
  return INLINE_SYSCALL_CALL (renameat2, oldfd, old, newfd, new, flags);
#else
  if (flags == 0)
    return __renameat (oldfd, old, newfd, new);
  /* __NR_renameat2 isn't currently supported in PM both on the kernel side and
     in glibc in fact.  */
# if defined __NR_renameat2 && ! (defined __e2k__ && defined __ptr128__)
  /* For non-zero flags, try the renameat2 system call.  */
  int ret = INLINE_SYSCALL_CALL (renameat2, oldfd, old, newfd, new, flags);
  if (ret != -1 || errno != ENOSYS)
    /* Preserve non-error/non-ENOSYS return values.  */
    return ret;
# endif
  /* No kernel (header) support for renameat2.  All flags are
     unknown.  */
  __set_errno (EINVAL);
  return -1;
#endif
}
