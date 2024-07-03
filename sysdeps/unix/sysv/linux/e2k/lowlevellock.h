/* Low-level lock implementation.  E2K futex-based version.
   Copyright (C) 2005-2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _LOWLEVELLOCK_H
#define _LOWLEVELLOCK_H	1

#include <atomic.h>
#include <elision-conf.h>
#include <lowlevellock-futex.h>
#include <time.h>



/* If LOCK is 0 (not acquired), set to 1 (acquired with no waiters) and return
   0.  Otherwise leave lock unchanged and return non-zero to indicate that the
   lock was not acquired.  */
#define __lll_trylock(lock)						\
  __glibc_unlikely ({							\
      int __res;							\
      int *__lock = (lock);						\
									\
      __res = atomic_compare_and_exchange_bool_acq (__lock, 1, 0);	\
      									\
      __res;								\
    })

#define lll_trylock(lock)	\
   __lll_trylock (&(lock))


/* If LOCK is 0 (not acquired), set to 2 (acquired, possibly with waiters) and
   return 0.  Otherwise leave lock unchanged and return non-zero to indicate
   that the lock was not acquired.  */
#define lll_cond_trylock(lock)						\
  __glibc_unlikely ({							\
      int __res;							\
      int *__lock = &(lock);						\
      __res = atomic_compare_and_exchange_bool_acq (__lock, 2, 0);	\
									\
      __res;								\
    })

extern void __lll_lock_wait_private (int *futex);
libc_hidden_proto (__lll_lock_wait_private)
extern void __lll_lock_wait (int *futex, int private);
libc_hidden_proto (__lll_lock_wait)

#if IS_IN (libc) || ! defined __LCC__
# define __lll_lock_wait_ex(f, p) __lll_lock_wait (f, p)
#else /* ! IS_IN (libc) && defined __LCC__  */
# define __lll_lock_wait_ex(f, p)		\
  {						\
    if (p == LLL_PRIVATE)			\
      __lll_lock_wait_private (f);		\
    else					\
      ((int *) 0)[0] = 0;			\
  }
#endif /* ! IS_IN (libc) && defined __LCC__  */

/* This is an expression rather than a statement even though its value is
   void, so that it can be used in a comma expression or as an expression
   that's cast to void.  */
/* The inner conditional compiles to a call to __lll_lock_wait_private if
   private is known at compile time to be LLL_PRIVATE, and to a call to
   __lll_lock_wait otherwise.  */
/* If FUTEX is 0 (not acquired), set to 1 (acquired with no waiters) and
   return.  Otherwise, ensure that it is >1 (acquired, possibly with waiters)
   and then block until we acquire the lock, at which point FUTEX will still be
   >1.  The lock is always acquired on return.  */
#define __lll_lock(futex, private)					\
  ((void)                                                               \
  ({									\
    int *__futex = (futex);						\
									\
    if (__glibc_unlikely						\
	(atomic_compare_and_exchange_bool_acq (__futex, 1, 0)))		\
      {									\
	if (__builtin_constant_p (private) && (private) == LLL_PRIVATE) \
	  __lll_lock_wait_private (__futex);				\
	else								\
	  __lll_lock_wait_ex (__futex, private);			\
      }									\
  }))

#define lll_lock(futex, private)	\
  __lll_lock (&(futex), private)


/* This is an expression rather than a statement even though its value is
   void, so that it can be used in a comma expression or as an expression
   that's cast to void.  */
/* Unconditionally set FUTEX to 2 (acquired, possibly with waiters).  If FUTEX
   was 0 (not acquired) then return.  Otherwise, block until the lock is
   acquired, at which point FUTEX is 2 (acquired, possibly with waiters).  The
   lock is always acquired on return.  */

/* According to its implementation in sysdeps/nptl/lowlevellock.h (note the
   absence of __lll_lock_wait_private (), this one is used only under
   IS_IN (libc) condition.  */
#define __lll_cond_lock(futex, private)		\
  ((void)					\
   ({						\
     int *__futex = (futex);			\
     if (__glibc_unlikely			\
	 (atomic_exchange_acq (__futex, 2)	\
	  != 0))				\
       __lll_lock_wait (__futex, private);	\
   }))

#define lll_cond_lock(futex, private) __lll_cond_lock (&(futex), private)


extern void __lll_lock_wake_private (int *futex);
libc_hidden_proto (__lll_lock_wake_private)
extern void __lll_lock_wake (int *futex, int private);
libc_hidden_proto (__lll_lock_wake)

#if IS_IN (libc) || ! defined __LCC__
# define __lll_lock_wake_ex(f, p) __lll_lock_wake (f, p)
#else /* ! IS_IN (libc) && defined __LCC__  */
# define __lll_lock_wake_ex(f, p)		\
  {						\
    if (p == LLL_PRIVATE)			\
      __lll_lock_wake_private (f);		\
    else					\
      ((int *) 0)[0] = 0;			\
  }
#endif /* ! IS_IN (libc) && defined __LCC__  */



/* This is an expression rather than a statement even though its value is
   void, so that it can be used in a comma expression or as an expression
   that's cast to void.  */
/* Unconditionally set FUTEX to 0 (not acquired), releasing the lock.  If FUTEX
   was >1 (acquired, possibly with waiters), then wake any waiters.  The waiter
   that acquires the lock will set FUTEX to >1.
   Evaluate PRIVATE before releasing the lock so that we do not violate the
   mutex destruction requirements.  Specifically, we need to ensure that
   another thread can destroy the mutex (and reuse its memory) once it
   acquires the lock and when there will be no further lock acquisitions;
   thus, we must not access the lock after releasing it, or those accesses
   could be concurrent with mutex destruction or reuse of the memory.  */
#define __lll_unlock(futex, private)					\
  ((void)								\
   ({									\
     int *__futex = (futex);						\
     int __private = (private);						\
     int __oldval = atomic_exchange_rel (__futex, 0);			\
									\
									\
     if (__glibc_unlikely (__oldval > 1))				\
       {								\
	 if (__builtin_constant_p (private) && (private) == LLL_PRIVATE) \
	   __lll_lock_wake_private (__futex);                           \
	 else                                                           \
	   __lll_lock_wake_ex (__futex, __private);			\
       }								\
   }))

#define lll_unlock(futex, private)	\
  __lll_unlock (&(futex), private)


#define lll_islocked(futex) \
  ((futex) != LLL_LOCK_INITIALIZER)


/* Our internal lock implementation is identical to the binary-compatible
   mutex implementation. */

/* Initializers for lock.  */
#define LLL_LOCK_INITIALIZER		(0)
#define LLL_LOCK_INITIALIZER_LOCKED	(1)

/* Stub Elision support.  */

#if ENABLE_ELISION_SUPPORT

# error "Elision is believed to be unsupported on e2k"

#else /* ! ENABLE_ELISION_SUPPORT */

# define lll_clocklock_elision(futex, adapt_count, clockid, abstime, private) \
  __futex_clocklock64 (&(futex), clockid, abstime, private)
# define lll_lock_elision(lock, try_lock, private)	\
  ({ lll_lock (lock, private); 0; })
# define lll_trylock_elision(a,t) lll_trylock(a)
# define lll_unlock_elision(a,b,c) ({ lll_unlock (a,c); 0; })
# define FORCE_ELISION(m, s)


#endif /* ! ENABLE_ELISION_SUPPORT */

#endif	/* lowlevellock.h */
