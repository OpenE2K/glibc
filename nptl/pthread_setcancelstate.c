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

#include <errno.h>
#include "pthreadP.h"
#include <atomic.h>
#include <libc-lockP.h>

int
__pthread_setcancelstate (int state, int *oldstate)
{
  volatile struct pthread *self;

  if (state < PTHREAD_CANCEL_ENABLE || state > PTHREAD_CANCEL_DISABLE)
    return EINVAL;

  self = THREAD_SELF;

  int oldval = atomic_load_relaxed (&self->cancelhandling);
  while (1)
    {
      int newval = (state == PTHREAD_CANCEL_DISABLE
		    ? oldval | CANCELSTATE_BITMASK
		    : oldval & ~CANCELSTATE_BITMASK);

      if (oldstate != NULL)
	*oldstate = ((oldval & CANCELSTATE_BITMASK)
		     ? PTHREAD_CANCEL_DISABLE : PTHREAD_CANCEL_ENABLE);

      if (oldval == newval)
	break;

      if (atomic_compare_exchange_weak_acquire (&self->cancelhandling,
						&oldval, newval))
	{
	  if (cancel_enabled_and_canceled_and_async (newval))
	    __do_cancel ();

	  break;
	}
    }

  return 0;
}
libc_hidden_def (__pthread_setcancelstate)

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__pthread_setcancelstate, pthread_setcancelstate)

#else /* defined __e2k__ && defined SHARED)  */

weak_alias (__pthread_setcancelstate, __pthread_setcancelstate_weak)

# include <shlib-compat.h>

/* This matches weak_alias in non-e2k-specific case.  */
versioned_symbol (libc, __pthread_setcancelstate_weak, pthread_setcancelstate,
		  GLIBC_2_0);

/* Unlike up-to-date glibc, in libpthread-2.29.so this symbol used to be
   GLOBAL.  */
compat_symbol (libpthread, __pthread_setcancelstate, pthread_setcancelstate,
	       GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED)  */
