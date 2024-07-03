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

#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>


/* Wait for a child matching PID to die.
   If PID is greater than 0, match any process whose process ID is PID.
   If PID is (pid_t) -1, match any process.
   If PID is (pid_t) 0, match any process with the
   same process group as the current process.
   If PID is less than -1, match any process whose
   process group is the absolute value of PID.
   If the WNOHANG bit is set in OPTIONS, and that child
   is not already dead, return (pid_t) 0.  If successful,
   return PID and store the dead child's status in STAT_LOC.
   Return (pid_t) -1 for errors.  If the WUNTRACED bit is set in OPTIONS,
   return status for stopped children; otherwise don't.  */
pid_t
#if ! (defined __e2k__ && defined SHARED)
__waitpid
#else /* defined __e2k__ && defined SHARED  */
static
__waitpid_local
#endif /* defined __e2k__ && defined SHARED  */
(pid_t pid, int *stat_loc, int options)
{
  return __wait4 (pid, stat_loc, options, NULL);
}

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (__waitpid)
weak_alias (__waitpid, waitpid)

#else /* defined __e2k__ && defined SHARED  */

#include <shlib-compat.h>

/* This alias ensures the creation of __GI_* () HIDDEN symbol (to which
   "aliasname" is transformed via libc_hidden_proto magic) intended for
   internal use within libc.so.  */
strong_alias (__waitpid_local, __waitpid)

strong_alias (__waitpid_local, __waitpid_strong)
versioned_symbol (libc, __waitpid_strong, __waitpid, GLIBC_2_2);

weak_alias (__waitpid_local, __waitpid_weak)
versioned_symbol (libc, __waitpid_weak, waitpid, GLIBC_2_2);
compat_symbol (libpthread, __waitpid_weak, waitpid, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
