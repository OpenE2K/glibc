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
#include <string.h>
#include "pthreadP.h"


int
__pthread_attr_setschedparam (pthread_attr_t *attr,
			      const struct sched_param *param)
{
  struct pthread_attr *iattr = (struct pthread_attr *) attr;

  int ret = check_sched_priority_attr (param->sched_priority,
				       iattr->schedpolicy);
  if (ret)
    return ret;

  /* Copy the new values.  */
  memcpy (&iattr->schedparam, param, sizeof (struct sched_param));

  /* Remember we set the value.  */
  iattr->flags |= ATTR_FLAG_SCHED_SET;

  return 0;
}

#if ! (defined __e2k__ && defined SHARED)

strong_alias (__pthread_attr_setschedparam, pthread_attr_setschedparam)

#else /* defined __e2k__ && defined SHARED  */
# include <shlib-compat.h>

/* Is the created versioned symbol going to become strong?  */
versioned_symbol (libc, __pthread_attr_setschedparam,
		  pthread_attr_setschedparam, GLIBC_2_2);

compat_symbol (libpthread, __pthread_attr_setschedparam,
	       pthread_attr_setschedparam, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
