/* Linux statx implementation.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <sys/stat.h>

#include "statx_generic.c"

int
statx (int fd, const char *path, int flags,
       unsigned int mask, struct statx *buf)
{
#if (defined __NR_statx							\
     && (! (defined __e2k__ && defined __ptr128__)			\
	 || defined __ptr128_new_abi__))
  int ret = INLINE_SYSCALL_CALL (statx, fd, path, flags, mask, buf);
# ifdef __ASSUME_STATX
  return ret;
# else
  if (ret == 0 || errno != ENOSYS)
    /* Preserve non-error/non-ENOSYS return values.  */
    return ret;
# endif
#endif
#ifndef __ASSUME_STATX
  return statx_generic (fd, path, flags, mask, buf);
#endif
}
