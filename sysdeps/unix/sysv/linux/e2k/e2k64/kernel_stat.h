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

/* Definition of `struct stat' used in the kernel */
struct kernel_stat
  {
    unsigned int st_dev;
    unsigned long int st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned int st_rdev;
    long int st_size;
    long int st_blksize;
    long int st_blocks;

    long int st_atime_sec;
    unsigned long int st_atime_nsec;
    
    long int st_mtime_sec;
    unsigned long int st_mtime_nsec;

    long int st_ctime_sec;
    unsigned long int st_ctime_nsec;
  };

/* Definition of `struct stat64' used in the kernel.  */
struct kernel_stat64
  {
    unsigned long st_dev;
    unsigned long st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned long st_rdev;
    unsigned long st_size;
    unsigned int st_blksize;
    unsigned int __unused1;
    unsigned long st_blocks;
    int st_atime_sec;
    unsigned int st_atime_nsec;
    int	st_mtime_sec;
    unsigned int st_mtime_nsec;
    int st_ctime_sec;
    unsigned int st_ctime_nsec;
  };

#define STAT_IS_KERNEL_STAT 0
#define XSTAT_IS_XSTAT64 1
#define STATFS_IS_STATFS64	1
