/* Copyright (c) 2009-2024 AO MCST.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
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

static inline void
__cp_kstat_stat64_t64 (const struct kernel_stat *kst, struct __stat64_t64 *st)
{
  /* Convert to current kernel version of `struct stat'.  */
  st->st_dev = kst->st_dev;
  st->st_ino = kst->st_ino;
  st->__pad1 = 0;
  st->st_mode = kst->st_mode;
  st->st_nlink = kst->st_nlink;
  st->st_uid = kst->st_uid;
  st->st_gid = kst->st_gid;
  st->st_rdev = kst->st_rdev;
  st->st_size = kst->st_size;
  st->st_blksize = kst->st_blksize;
  st->__glibc_reserved1 = 0;
  st->st_blocks = kst->st_blocks;
  st->st_atim.tv_sec = kst->st_atime_sec;
  st->st_atim.tv_nsec = kst->st_atime_nsec;
  st->st_mtim.tv_sec = kst->st_mtime_sec;
  st->st_mtim.tv_nsec = kst->st_mtime_nsec;
  st->st_ctim.tv_sec = kst->st_ctime_sec;
  st->st_ctim.tv_nsec = kst->st_ctime_nsec;
}
