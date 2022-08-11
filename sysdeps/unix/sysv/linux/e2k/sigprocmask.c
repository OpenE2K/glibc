/* Copyright (C) 1997-2013 Free Software Foundation, Inc.
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

/* FIXME: the contents of this file has been copied for the most part from
   <nptl/sysdeps/pthread/sigprocmask.c> and
   <sysdeps/unix/sysv/linux/sigprocmask.c>. The reason for that is to add
   __NR_fast_sys_siggetmask syscall invocation at the very end. Such an
   approach may cause additional trouble if one of the original files changes
   one day.  */

/* The underlying string is copied from <nptl/sysdeps/pthread/sigprocmask.c>  */
#include <nptl/pthreadP.h>

/* Here come the contents of <sysdeps/unix/sysv/linux/sigprocmask.c>, the
   additions are related to fast system call invocation.  */
#include <errno.h>
#include <signal.h>
#include <string.h>	/* Neede for string function builtin redirection.  */
#include <unistd.h>

#include <sysdep.h>
#include <sys/syscall.h>

#include <kernel-features.h>

#ifdef HAVE_SIGGETMASK_VSYSCALL
# define HAVE_VSYSCALL
#endif /* HAVE_SIGGETMASK_VSYSCALL  */
#include <sysdep-vdso.h>


/* Get and/or change the set of blocked signals.  */
int
__sigprocmask (how, set, oset)
     int how;
     const sigset_t *set;
     sigset_t *oset;
{
#ifdef SIGCANCEL
  sigset_t local_newmask;

  /* The only thing we have to make sure here is that SIGCANCEL and
     SIGSETXID are not blocked.  */
  if (set != NULL
      && (__builtin_expect (__sigismember (set, SIGCANCEL), 0)
# ifdef SIGSETXID
	  || __builtin_expect (__sigismember (set, SIGSETXID), 0)
# endif
	  ))
    {
      local_newmask = *set;
      __sigdelset (&local_newmask, SIGCANCEL);
# ifdef SIGSETXID
      __sigdelset (&local_newmask, SIGSETXID);
# endif
      set = &local_newmask;
    }
#endif

  /* The underlying ifdef accounts for the fact that we don't actually have an
     ordinary syscall for siggetmask.  */
#ifdef HAVE_VSYSCALL
  /* If `set' is NULL we prefer to use `siggetmask' (see Bug #60302,
     Comment #15).  We don't have a "slow" version of `siggetmask' to rely on
     in case fast syscalls are not available, therefore INLINE_VSYSCALL cannot
     be used here. */
  if (set == NULL
      && (__libc_e2k_fast_syscalls & (1 << __NR_fast_sys_siggetmask)))
    return __INLINE_SYSCALL (VSYSCALL, siggetmask, 2, oset, _NSIG / 8);
#endif /* HAVE_VSYSCALL  */

  return INLINE_SYSCALL (rt_sigprocmask, 4, how, set, oset, _NSIG / 8);
}
libc_hidden_def (__sigprocmask)
weak_alias (__sigprocmask, sigprocmask)
