/* Copyright (C) 2002-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

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
#include "pthreadP.h"
#include <lowlevellock.h>
#include <sparc-nptl.h>

int
pthread_barrier_destroy (pthread_barrier_t *barrier)
{
  union sparc_pthread_barrier *ibarrier;
  int result = EBUSY;

  ibarrier = (union sparc_pthread_barrier *) barrier;

  int private = ibarrier->s.pshared ? LLL_SHARED : LLL_PRIVATE;

  lll_lock (ibarrier->b.lock, private);

  if (__glibc_likely (ibarrier->b.left == ibarrier->b.init_count))
    /* The barrier is not used anymore.  */
    result = 0;
  else
    /* Still used, return with an error.  */
    lll_unlock (ibarrier->b.lock, private);

  return result;
}
