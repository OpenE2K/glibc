/* Machine-specific pthread type layouts.  E2K version.
   Copyright (C) 2003, 2004, 2005, 2006, 2007 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 2003.

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

#ifndef _BITS_PTHREADTYPES_ARCH_H
#define _BITS_PTHREADTYPES_ARCH_H	1

#include <bits/wordsize.h>

#if __WORDSIZE == 64

# if ! defined __ptr128__
#  define __SIZEOF_PTHREAD_ATTR_T 56
# define __SIZEOF_PTHREAD_MUTEX_T 40
# else /* defined __ptr128__  */
#  define __SIZEOF_PTHREAD_ATTR_T 96
# define __SIZEOF_PTHREAD_MUTEX_T 64
# endif /* defined __ptr128__  */

# define __SIZEOF_PTHREAD_RWLOCK_T 56
# define __SIZEOF_PTHREAD_BARRIER_T 32
#else
# define __SIZEOF_PTHREAD_ATTR_T 36
# define __SIZEOF_PTHREAD_MUTEX_T 24
# define __SIZEOF_PTHREAD_RWLOCK_T 32
# define __SIZEOF_PTHREAD_BARRIER_T 20
#endif


#define __SIZEOF_PTHREAD_MUTEXATTR_T 4
#define __SIZEOF_PTHREAD_COND_T 48
#define __SIZEOF_PTHREAD_CONDATTR_T 4
#define __SIZEOF_PTHREAD_RWLOCKATTR_T 8
#define __SIZEOF_PTHREAD_BARRIERATTR_T 4

/* Definitions for internal mutex struct.  */
#define __PTHREAD_COMPAT_PADDING_MID
#define __PTHREAD_COMPAT_PADDING_END
#define __PTHREAD_MUTEX_LOCK_ELISION    0

#if __WORDSIZE == 32
# define __PTHREAD_MUTEX_NUSERS_AFTER_KIND  1
# define __PTHREAD_MUTEX_USE_UNION          1
#else /* __WORDSIZE != 32  */
# define __PTHREAD_MUTEX_NUSERS_AFTER_KIND  0
# define __PTHREAD_MUTEX_USE_UNION          0
#endif /* __WORDSIZE != 32  */

#define __LOCK_ALIGNMENT
#define __ONCE_ALIGNMENT


struct __pthread_rwlock_arch_t
{
  unsigned int __readers;
  unsigned int __writers;
  unsigned int __wrphase_futex;
  unsigned int __writers_futex;
  unsigned int __pad3;
  unsigned int __pad4;
#if __WORDSIZE == 64
  int __cur_writer;
  int __shared;
  unsigned long int __pad1;
  unsigned long int __pad2;
  /* FLAGS must stay at this position in the structure to maintain
     binary compatibility.  */
  unsigned int __flags;
#else
  /* FLAGS must stay at this position in the structure to maintain
     binary compatibility.  */
  unsigned char __flags;
  unsigned char __shared;
  unsigned char __pad2;
  unsigned char __pad1;
  int __cur_writer;
#endif
};

#define __PTHREAD_RWLOCK_ELISION_EXTRA 0

#endif	/* bits/pthreadtypes-arch.h */
