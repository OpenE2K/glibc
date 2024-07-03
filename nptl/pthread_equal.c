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


int
__pthread_equal (pthread_t thread1, pthread_t thread2)
{
  return thread1 == thread2;
}

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__pthread_equal, pthread_equal)

#else /* defined __e2k__ && defined SHARED  */

#include <shlib-compat.h>

weak_alias (__pthread_equal, __pthread_equal_weak)
versioned_symbol (libc, __pthread_equal_weak, pthread_equal,
		  GLIBC_2_2);

compat_symbol (libpthread, __pthread_equal_weak, pthread_equal,
	       GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
