/* Check if clone (CLONE_THREAD) does not call exit_group (BZ #21512)
   Copyright (C) 2017-2019 Free Software Foundation, Inc.
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

#include <string.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/futex.h>

#include <stackinfo.h>  /* For _STACK_GROWS_{UP,DOWN}.  */
#include <support/check.h>

/* Test if clone call with CLONE_THREAD does not call exit_group.  The 'f'
   function returns '1', which will be used by clone thread to call the
   'exit' syscall directly.  If _exit is used instead, exit_group will be
   used and thus the thread group will finish with return value of '1'
   (where '2' from main thread is expected.  */

static int
f (void *a)
{
  return 1;
}

/* Futex wait for TID argument, similar to pthread_join internal
   implementation.  */
#define wait_tid(tid) \
  do {					\
    __typeof (tid) __tid;		\
    while ((__tid = (tid)) != 0)	\
      futex_wait (&(tid), __tid);	\
  } while (0)

static inline int
futex_wait (int *futexp, int val)
{
#ifdef __ptr128__
  /* Why don't they take care of TIMEOUT for FUTEX_WAIT in "ordinary"
     modes?  */
  struct timespec *timeout = NULL;
#endif
  return syscall (__NR_futex, futexp, FUTEX_WAIT, val
#ifdef __ptr128__
		  , &timeout
#endif
		  );
}

static int
do_test (void)
{
  char st[1024] __attribute__ ((aligned));
  int clone_flags = CLONE_THREAD;
  /* Minimum required flags to used along with CLONE_THREAD.  */
  clone_flags |= CLONE_VM | CLONE_SIGHAND;
  /* We will used ctid to call on futex to wait for thread exit.  */
  clone_flags |= CLONE_CHILD_CLEARTID;
  pid_t ctid, tid;

#ifdef __ia64__
  extern int __clone2 (int (*__fn) (void *__arg), void *__child_stack_base,
		       size_t __child_stack_size, int __flags,
		       void *__arg, ...);
  tid = __clone2 (f, st, sizeof (st), clone_flags, NULL, /* ptid */ NULL,
		  /* tls */ NULL, &ctid);
#else
#if _STACK_GROWS_DOWN
  tid = clone (f, st + sizeof (st), clone_flags, NULL, /* ptid */ NULL,
	       /* tls */ NULL, &ctid);
#elif _STACK_GROWS_UP
  tid = clone (f, st, clone_flags, NULL, /* ptid */ NULL, /* tls */ NULL,
	       &ctid);
#else
#error "Define either _STACK_GROWS_DOWN or _STACK_GROWS_UP"
#endif
#endif
  if (tid == -1)
    FAIL_EXIT1 ("clone failed: %m");

  ctid = tid;
  wait_tid (ctid);

  return 2;
}

#define EXPECTED_STATUS 2
#include <support/test-driver.c>
