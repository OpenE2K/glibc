/* Copyright (C) 2002-2022 Free Software Foundation, Inc.
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

#include "pthreadP.h"
#include <shlib-compat.h>

int
___pthread_join (pthread_t threadid, void **thread_return)
{
  return __pthread_clockjoin_ex (threadid, thread_return, 0 /* Ignored */,
				 NULL, true);
}
versioned_symbol (libc, ___pthread_join, pthread_join, GLIBC_2_34);
libc_hidden_ver (___pthread_join, __pthread_join)
#ifndef SHARED
strong_alias (___pthread_join, __pthread_join)
#endif

#if OTHER_SHLIB_COMPAT (libpthread, GLIBC_2_0, GLIBC_2_34)
# if ! defined __e2k__
compat_symbol (libc, ___pthread_join, pthread_join, GLIBC_2_0);
# else /* defined __e2k__  */
/* No pthread_join@... actually existed in libc-2.29.so, but we need
   pthread_join@GLIBC_2.0 with exactly the same version it had in
   libpthread-2.29.so which is why "libpthread" is specified here instead
   of "libc". Here I also preserve its original WEAKness. But should this
   actually be done taking into account that they do not take care about
   that above?  */
weak_alias (___pthread_join, ___pthread_join_weak)
compat_symbol (libpthread, ___pthread_join_weak, pthread_join, GLIBC_2_0);
# endif /* defined __e2k__  */
#endif
