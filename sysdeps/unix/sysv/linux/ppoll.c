/* Copyright (C) 2006-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2006.

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

#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/poll.h>
#include <sysdep-cancel.h>


int
ppoll (struct pollfd *fds, nfds_t nfds, const struct timespec *timeout,
       const sigset_t *sigmask)
{
  /* The Linux kernel can in some situations update the timeout value.
     We do not want that so use a local variable.  */
  struct timespec tval;
  if (timeout != NULL)
    {
      tval = *timeout;
      timeout = &tval;
    }

#if defined __ptr128__ && ! defined __ptr128_new_abi__
  /* FIXME: the arguments to ppoll () are passed entirely on registers, which
     has been achieved by passing NFDS first and excluding the trailing `_NSIG
     / 8', which SHOULD be passed to the Kernel in fact. Consider packing them
     into a struct instead.  */
  return SYSCALL_CANCEL (ppoll, nfds, fds, timeout, sigmask);
#else /* ! defined __ptr128__ ||  defined __ptr128_new_abi__  */
  return SYSCALL_CANCEL (ppoll, fds, nfds, timeout, sigmask, _NSIG / 8);
#endif /* ! defined __ptr128__ ||  defined __ptr128_new_abi__  */
}
libc_hidden_def (ppoll)
