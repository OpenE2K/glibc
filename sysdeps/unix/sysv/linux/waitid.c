/* Linux implementation of waitid.
   Copyright (C) 2004-2019 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#include <stddef.h>
#include <errno.h>
#include <sys/wait.h>
#include <sysdep-cancel.h>

int
__waitid (idtype_t idtype, id_t id, siginfo_t *infop, int options)
{
  /* The unused fifth argument is a `struct rusage *' that we could
     pass if we were using waitid to simulate wait3/wait4.  */
#if defined __ptr128__ && ! defined __ptr128_new_abi__
  return SYSCALL_CANCEL (waitid, idtype, id, options, infop, NULL);
#else /* ! (defined __ptr128__ && ! defined __ptr128_new_abi__)  */
  return SYSCALL_CANCEL (waitid, idtype, id, infop, options, NULL);
#endif /* ! (defined __ptr128__ && ! defined __ptr128_new_abi__)  */
}
weak_alias (__waitid, waitid)
strong_alias (__waitid, __libc_waitid)
