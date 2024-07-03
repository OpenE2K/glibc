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

/* Definition of `struct rlimit' used in the kernel */
struct kernel_rlimit
  {
    unsigned long long rlim_cur;
    unsigned long long rlim_max;
  };

#define convert_rlimit(dst,src) \
    ((dst)->rlim_cur = (src)->rlim_cur, \
     (dst)->rlim_max = (src)->rlim_max)


/* Definition of `struct rusage' used in the kernel */
struct kernel_rusage
  {
    struct kernel_timeval ru_utime;
    struct kernel_timeval ru_stime;
    long long ru_maxrss;
    long long ru_ixrss;
    long long ru_idrss;
    long long ru_isrss;
    long long ru_minflt;
    long long ru_majflt;
    long long ru_nswap;
    long long ru_inblock;
    long long ru_oublock;
    long long ru_msgsnd;
    long long ru_msgrcv;
    long long ru_nsignals;
    long long ru_nvcsw;
    long long ru_nivcsw;
  };

#define convert_rusage(dst,src) \
    (convert_timeval (&((dst)->ru_utime), &((src)->ru_utime)), \
     convert_timeval (&((dst)->ru_stime), &((src)->ru_stime)), \
     (dst)->ru_maxrss = (src)->ru_maxrss, \
     (dst)->ru_ixrss = (src)->ru_ixrss, \
     (dst)->ru_idrss = (src)->ru_idrss, \
     (dst)->ru_isrss = (src)->ru_isrss, \
     (dst)->ru_minflt = (src)->ru_minflt, \
     (dst)->ru_majflt = (src)->ru_majflt, \
     (dst)->ru_nswap = (src)->ru_nswap, \
     (dst)->ru_inblock = (src)->ru_inblock, \
     (dst)->ru_oublock = (src)->ru_oublock, \
     (dst)->ru_msgsnd = (src)->ru_msgsnd, \
     (dst)->ru_msgrcv = (src)->ru_msgrcv, \
     (dst)->ru_nsignals = (src)->ru_nsignals, \
     (dst)->ru_nvcsw = (src)->ru_nvcsw, \
     (dst)->ru_nivcsw = (src)->ru_nivcsw)
