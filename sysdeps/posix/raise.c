/* Copyright (C) 1991-2022 Free Software Foundation, Inc.
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

#include <signal.h>
#include <errno.h>
#include <pthread.h>

/* Raise the signal SIG.  */
int
#if ! (defined __e2k__ && defined SHARED)
raise
#else /* defined __e2k__ && defined SHARED  */
static
__raise_local
#endif /* defined __e2k__ && defined SHARED  */
(int sig)
{
  int ret = __pthread_kill (__pthread_self (), sig);
  if (ret != 0)
    {
      __set_errno (ret);
      ret = -1;
    }
  return ret;
}

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (raise)
weak_alias (raise, gsignal)

#else /* defined __e2k__ && defined SHARED  */

#include <shlib-compat.h>

/* This alias ensures the creation of __GI_* () HIDDEN symbol (to which
   "aliasname" is transformed via libc_hidden_proto magic) intended for
   internal use within libc.so.  */
strong_alias (__raise_local, raise)

strong_alias (__raise_local, __raise_strong)
versioned_symbol (libc, __raise_strong, raise, GLIBC_2_0);
compat_symbol (libpthread, __raise_strong, raise, GLIBC_2_0);

weak_alias (__raise_local, gsignal)

#endif /* defined __e2k__ && defined SHARED  */
