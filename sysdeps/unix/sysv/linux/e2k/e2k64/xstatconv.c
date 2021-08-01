/* Convert between the kernel's `struct stat' format, and libc's.
   Copyright (C) 1991, 1995, 1996, 1997, 2000, 2002, 2003, 2006
   Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <kernel_stat.h>
#include <string.h>
#include <kernel-features.h>

int
__xstat_conv (int vers, struct kernel_stat *kbuf, void *ubuf)
{
  switch (vers)
    {
    case _STAT_VER_KERNEL:
      /* Nothing to do.  The struct is in the form the kernel expects.
         We should have short-circuted before we got here, but for
         completeness... */
      *(struct kernel_stat *) ubuf = *kbuf;
      break;

    case _STAT_VER_LINUX:
      {
	struct stat *buf = ubuf;

	/* Convert to current kernel version of `struct stat'.  */
	buf->st_dev = kbuf->st_dev;
        buf->st_ino = kbuf->st_ino;
        buf->__pad1 = 0;
	buf->st_mode = kbuf->st_mode;
	buf->st_nlink = kbuf->st_nlink;
	buf->st_uid = kbuf->st_uid;
	buf->st_gid = kbuf->st_gid;
	buf->st_rdev = kbuf->st_rdev;
	buf->st_size = kbuf->st_size;
	buf->st_blksize = kbuf->st_blksize;
        buf->__glibc_reserved1 = 0;
	buf->st_blocks = kbuf->st_blocks;
	buf->st_atim.tv_sec = kbuf->st_atime_sec;
	buf->st_atim.tv_nsec = kbuf->st_atime_nsec;
	buf->st_mtim.tv_sec = kbuf->st_mtime_sec;
	buf->st_mtim.tv_nsec = kbuf->st_mtime_nsec;
	buf->st_ctim.tv_sec = kbuf->st_ctime_sec;
	buf->st_ctim.tv_nsec = kbuf->st_ctime_nsec;
      }
      break;

    default:
      __set_errno (EINVAL);
      return -1;
    }

  return 0;
}
