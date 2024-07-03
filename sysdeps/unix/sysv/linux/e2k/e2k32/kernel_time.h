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

/* Definition of `struct timeval' used in the kernel */
struct kernel_timeval
  {
    long long tv_sec;
    long long tv_usec;
  };

#define convert_timeval(dst,src) \
    ((dst)->tv_sec = (src)->tv_sec, \
     (dst)->tv_usec = (src)->tv_usec)


/* Definition of `struct itimerval' used in the kernel */
struct kernel_itimerval
  {
    struct kernel_timeval it_interval;
    struct kernel_timeval it_value;
  };

#define convert_itimerval(dst,src) \
    (convert_timeval (&((dst)->it_interval), &((src)->it_interval)), \
     convert_timeval (&((dst)->it_value), &((src)->it_value)))


/* Definition of `struct tms' used in the kernel */
struct kernel_tms
  {
    long long tms_utime;
    long long tms_stime;
    long long tms_cutime;
    long long tms_cstime;
  };

#define convert_tms(dst,src) \
    ((dst)->tms_utime = (src)->tms_utime, \
     (dst)->tms_stime = (src)->tms_stime, \
     (dst)->tms_cutime = (src)->tms_cutime, \
     (dst)->tms_cstime = (src)->tms_cstime)


/* Definition of `struct utimbuf' used in the kernel */
struct kernel_utimbuf
  {
    long long actime;
    long long modtime;
  };

#define convert_utimbuf(dst,src) \
    ((dst)->actime = (src)->actime, \
     (dst)->modtime = (src)->modtime)


/* Definition of `struct timespec' used in the kernel */
struct kernel_timespec
  {
    long long tv_sec;
    long long tv_nsec;
  };

#define convert_timespec(dst,src) \
    ((dst)->tv_sec = (src)->tv_sec, \
     (dst)->tv_nsec = (src)->tv_nsec)
