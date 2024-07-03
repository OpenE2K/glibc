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

#include <sys/wait.h>

/* Wait for a child to die.  When one does, put its status in *STAT_LOC
   and return its process ID.  For errors, return (pid_t) -1.  */
__pid_t
#if ! (defined __e2k__ && defined SHARED)
__wait
#else /* defined __e2k__ && defined SHARED  */
/* Unlike __waitpid () __wait is NOT subject to libc_hidden_{proto & def}
   magic (see `include/sys/wait.h') because of which I need to change its
   name here to create the versioned __wait@@GLIBC_2_2 below.  */
__wait_strong
#endif /* defined __e2k__ && defined SHARED  */
(int *stat_loc)
{
  return __waitpid (WAIT_ANY, stat_loc, 0);
}

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__wait, wait)

#else /* defined __e2k__ && defined SHARED  */

# include <shlib-compat.h>

versioned_symbol (libc, __wait_strong,  __wait, GLIBC_2_2);
weak_alias (__wait_strong, __wait_weak)
versioned_symbol (libc, __wait_weak,  wait, GLIBC_2_2);

/* In libpthread-2.29.so both {,__}wait@GLIBC_2.0 were WEAK unlike upstream
   where only wait@GLIBC_2.0 is (see a few lines above).  */
compat_symbol (libpthread, __wait_weak, __wait, GLIBC_2_0);
compat_symbol (libpthread, __wait_weak, wait, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
