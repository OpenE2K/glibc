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
#include <signal.h>
#include <internal-signals.h>

/* If ACT is not NULL, change the action for SIG to *ACT.
   If OACT is not NULL, put the old action for SIG in *OACT.  */
int
#if IS_IN (rtld) || ! (defined __e2k__ && defined SHARED)
__sigaction
#else /* ! IS_IN (rtld) && (defined __e2k__ && defined SHARED)  */
static
__sigaction_local
#endif /* ! IS_IN (rtld) && (defined __e2k__ && defined SHARED)  */
(int sig, const struct sigaction *act, struct sigaction *oact)
{
  if (sig <= 0 || sig >= NSIG || __is_internal_signal (sig))
    {
      __set_errno (EINVAL);
      return -1;
    }

  return __libc_sigaction (sig, act, oact);
}


#if IS_IN (rtld) || ! (defined __e2k__ && defined SHARED)

/* Prevent creation of GLOBAL __sigaction and WEAK sigaction without an
   explicitly specified default version in SHARED case for e2k unless
   this file is compiled for ld.so.  */
libc_hidden_def (__sigaction)
weak_alias (__sigaction, sigaction)

#else /* ! IS_IN (rtld) && (defined __e2k__ && defined SHARED)  */

#include <shlib-compat.h>

/* This alias ensures the creation of __GI_* () HIDDEN symbol (to which
   "aliasname" is transformed via libc_hidden_proto magic) intended for
   internal use within libc.so.  */
strong_alias (__sigaction_local, __sigaction)

strong_alias (__sigaction_local, __sigaction_strong)
weak_alias (__sigaction_strong, __sigaction_weak)

/* {__,}sigaction are quite conventionally WEAK and GLOBAL respectively both
   in GLIBC_2.{2,0}.  */

versioned_symbol (libc, __sigaction_strong, __sigaction, GLIBC_2_0);
versioned_symbol (libc, __sigaction_weak, sigaction, GLIBC_2_0);

compat_symbol (libpthread, __sigaction_strong, __sigaction, GLIBC_2_0);
compat_symbol (libpthread, __sigaction_weak, sigaction, GLIBC_2_0);

#endif /* ! IS_IN (rtld) && (defined __e2k__ && defined SHARED)  */
