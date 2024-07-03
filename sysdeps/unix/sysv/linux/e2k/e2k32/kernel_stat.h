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

struct timespec_kernel
{
    long long int tv_sec;		/* Seconds.  */
    unsigned long long int tv_nsec;		/* Nanoseconds.  */
};

struct kernel_stat
  {
    unsigned long long int st_dev;
    unsigned long long int st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned long long int st_rdev;
    unsigned long long int st_size;
    unsigned long long int st_blksize;
    unsigned long long int st_blocks;
    struct timespec_kernel st_atim;
    struct timespec_kernel st_mtim;
    struct timespec_kernel st_ctim;
  };

#define _HAVE_STAT___UNUSED1
#define _HAVE_STAT64___UNUSED1
#define _HAVE_STAT_NSEC
#define _HAVE_STAT64_NSEC

#define STAT_IS_KERNEL_STAT 0
/* We have implicitly used these values for years.  */
#define XSTAT_IS_XSTAT64	0
#define STATFS_IS_STATFS64	0
