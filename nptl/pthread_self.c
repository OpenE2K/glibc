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
#include <tls.h>

pthread_t
__pthread_self (void)
{
  return (pthread_t) THREAD_SELF;
}
libc_hidden_def (__pthread_self)

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__pthread_self, pthread_self)

#else /* defined __e2k__ && defined SHARED  */
# include <shlib-compat.h>

/* pthread_self@GLIBC_2.2 used to be "strong", but has become WEAK as a result
   of recent changes.  */
weak_alias (__pthread_self, __pthread_self_weak)
versioned_symbol (libc, __pthread_self_weak, pthread_self, GLIBC_2_2);

/* Let pthread_self@GLIBC_2.0 remain "strong" for compatibility with
   libpthread-2.29.so.  */
compat_symbol (libpthread, __pthread_self, pthread_self, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
