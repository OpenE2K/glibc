/* Copyright (c) 2009-2024 AO MCST.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
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

#include <signal.h>
#include <sysdep.h>

extern void makecontext_helper (void);

int
__makecontext_e2k (ucontext_t *ucp, void (*func) (void), int argc, ...)
{
#if !defined __ptr128__
  unsigned long long buf[argc + 4];
#else /* defined __ptr128__  */
  void * buf[argc + 3];
#endif /* defined __ptr128__  */

#if ! defined __ptr128__
  buf[0] = (unsigned long long) (unsigned long) ucp;
  buf[1] = (unsigned long long) (unsigned long) func;
  /* Zero out an otherwise unused extra subsidiary argument just to ensure
     16-byte alignment of the first target function's parameter.  */
  buf[3] = 0x0ULL;
#else /* defined __ptr128__  */
  buf[0] = (void *) ucp;
  buf[1] = (void *) func;
#endif /* defined __ptr128__  */

  *((int *) &buf[2]) = argc;
  memcpy ((void *) &buf[
#if ! defined __ptr128__
			4
#else /* defined __ptr128__  */
			3
#endif /* defined __ptr128__  */
			],
	  (void *) (&argc
#if ! defined __ptr128__
		    + 2
#else /* defined __ptr128__  */
		    + 4
#endif /* defined __ptr128__  */
		    ),
	  (
#if ! defined __ptr128__
	   8
#else /* defined __ptr128__  */
	   16
#endif /* defined __ptr128__  */
	   ) * argc);
  
  return INLINE_SYSCALL (makecontext, 5, ucp, &makecontext_helper,
#if ! defined __ptr128__
			 8 * (argc + 4), (unsigned long) buf,
#else /* defined __ptr128__  */
			 /* Evaluate the size of the parameters' area and the
			    pointer to the first argument in `...' according
			    to the new PM ABI. When calculating the latter
			    take into account that `sizeof (argc) == 4'.  */
			 16UL * (argc + 3), buf,
#endif /* defined __ptr128__  */
			 _NSIG / 8);
}

weak_alias (__makecontext_e2k, makecontext_e2k)
