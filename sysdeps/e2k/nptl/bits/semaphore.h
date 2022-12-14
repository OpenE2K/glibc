/* Copyright (C) 2002, 2004 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _SEMAPHORE_H
# error "Never use <bits/semaphore.h> directly; include <semaphore.h> instead."
#endif

#include <bits/wordsize.h>

#if __WORDSIZE == 64
# define __SIZEOF_SEM_T	32
#else
# define __SIZEOF_SEM_T	16
#endif


/* Value returned if `sem_open' failed.  */
#define SEM_FAILED      ((sem_t *) 0)


/* FIXME: it would be better to define __NEW_SEM_ALIGN_32 for 32-bit mode here
   rather than in atomic-machine.h, however, it's impossible to guarantee that
   this file is included BEFORE internaltypes.h containing the definition of
   `struct new_sem'.  */

typedef union
{
  char __size[__SIZEOF_SEM_T];
  long int __align;
} sem_t;
