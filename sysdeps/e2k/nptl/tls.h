/* Definitions for thread-local data handling.  NPTL/e2k version.
   Copyright (C) 2003, 2005, 2006, 2007 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _TLS_H
#define _TLS_H 1

#if defined __ptr128__
# define __TLS_REGISTER %g12
#else /* ! __ptr128__  */
# define __TLS_REGISTER %g13
#endif /* ! __ptr128__  */

#include <dl-sysdep.h>
#ifndef __ASSEMBLER__
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <list.h>
# include <kernel-features.h>
# include <dl-dtv.h>

typedef struct
{
  dtv_t *dtv;
  void *__private;
} tcbhead_t;

#else /* __ASSEMBLER__ */
# include <tcb-offsets.h>
#endif /* __ASSEMBLER__ */

#ifndef __ASSEMBLER__
/* Get system call information.  */
# include <sysdep.h>

register unsigned long long __g13 __asm__(__p2(__TLS_REGISTER));
register struct pthread *__thread_self __asm__(__p2(__TLS_REGISTER));

/* This is the size of the initial TCB. */
# define TLS_INIT_TCB_SIZE sizeof (tcbhead_t)

/* Alignment requirements for the initial TCB.  */
# define TLS_INIT_TCB_ALIGN __alignof__ (tcbhead_t)

/* This is the size of the TCB.  */
# define TLS_TCB_SIZE sizeof (tcbhead_t)

/* This is the size we need before TCB. */
# define TLS_PRE_TCB_SIZE \
  (sizeof (struct pthread)						\
   + (PTHREAD_STRUCT_END_PADDING < 2 * sizeof (uintptr_t)		\
      ? ((2 * sizeof (uintptr_t) + __alignof__ (struct pthread) - 1)	\
	 & ~(__alignof__ (struct pthread) - 1))				\
      : 0))

/* Alignment requirements for the TCB.  */
# define TLS_TCB_ALIGN __alignof__ (struct pthread)

/* The DTV is allocated at the TP; the TCB is placed elsewhere.  */
# define TLS_DTV_AT_TP	1
# define TLS_TCB_AT_TP	0

/* Get the thread descriptor definition.  */
# include <nptl/descr.h>

/* Install the dtv pointer.  The pointer passed is to the element with
   index -1 which contain the length.  */
# define INSTALL_DTV(descr, dtvp) \
  ((tcbhead_t *) (descr))->dtv = (dtvp) + 1

/* Install new dtv for current thread.  */
# define INSTALL_NEW_DTV(DTV) \
  (((tcbhead_t *) __thread_self)->dtv = (DTV))

/* Return dtv of given thread descriptor.  */
# define GET_DTV(descr) \
  (((tcbhead_t *) (descr))->dtv)

/* Code to initially initialize the thread pointer.  */
# define TLS_INIT_TP(descr) \
  (__g13 = 0ULL, __thread_self = (__typeof (__thread_self)) (descr), NULL)

/* Value passed to `clone ()' for initialization of the thread register.  */
# define TLS_DEFINE_INIT_TP(tp, pd) \
  void *tp = (char *) (pd) + TLS_PRE_TCB_SIZE

/* Return the address of the dtv for the current thread.  */
# define THREAD_DTV() \
  (((tcbhead_t *) __thread_self)->dtv)

/* Return the thread descriptor for the current thread.  */
#define THREAD_SELF \
  ((struct pthread *) ((char *) __thread_self - TLS_PRE_TCB_SIZE))

/* Magic for libthread_db to know how to do THREAD_SELF.  */
# define DB_THREAD_SELF REGISTER (64, 64, 14 * 8, -TLS_PRE_TCB_SIZE)


/* Access to data in the thread descriptor is easy.  */
#define THREAD_GETMEM(descr, member) \
  descr->member
#define THREAD_GETMEM_NC(descr, member, idx) \
  descr->member[idx]
#define THREAD_SETMEM(descr, member, value) \
  descr->member = (value)
#define THREAD_SETMEM_NC(descr, member, idx, value) \
  descr->member[idx] = (value)

/* Set the stack guard field in TCB head.  */
#define THREAD_SET_STACK_GUARD(value) \
  (((uintptr_t *) __thread_self)[-1] = (value))
#define THREAD_COPY_STACK_GUARD(descr) \
  (((uintptr_t *) ((char *) (descr) + TLS_PRE_TCB_SIZE))[-1] \
   = ((uintptr_t *) __thread_self)[-1])

/* Set the pointer guard field in TCB head.  */
#define THREAD_GET_POINTER_GUARD() \
  (((uintptr_t *) __thread_self)[-2])
#define THREAD_SET_POINTER_GUARD(value) \
  (((uintptr_t *) __thread_self)[-2] = (value))
#define THREAD_COPY_POINTER_GUARD(descr) \
  (((uintptr_t *) ((char *) (descr) + TLS_PRE_TCB_SIZE))[-2] \
   = THREAD_GET_POINTER_GUARD ())

/* Get and set the global scope generation counter in struct pthread.  */
#define THREAD_GSCOPE_IN_TCB      1
#define THREAD_GSCOPE_FLAG_UNUSED 0
#define THREAD_GSCOPE_FLAG_USED   1
#define THREAD_GSCOPE_FLAG_WAIT   2
#define THREAD_GSCOPE_RESET_FLAG() \
  do									     \
    { int __res								     \
	= atomic_exchange_rel (&THREAD_SELF->header.gscope_flag,	     \
			       THREAD_GSCOPE_FLAG_UNUSED);		     \
      if (__res == THREAD_GSCOPE_FLAG_WAIT)				     \
	lll_futex_wake (&THREAD_SELF->header.gscope_flag, 1, LLL_PRIVATE);   \
    }									     \
  while (0)
#define THREAD_GSCOPE_SET_FLAG() \
  do									     \
    {									     \
      THREAD_SELF->header.gscope_flag = THREAD_GSCOPE_FLAG_USED;	     \
      atomic_write_barrier ();						     \
    }									     \
  while (0)
#define THREAD_GSCOPE_WAIT() \
  GL(dl_wait_lookup_done) ()

#endif /* __ASSEMBLER__ */

#endif	/* tls.h */
