/* Copyright (C) 2008-2019 Free Software Foundation, Inc.
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

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include <tst-stack-align.h>

ucontext_t ucp, ucp2;
char st1[262144] __attribute__((aligned (16)));

#ifdef __e2k__
static void
free_ucp (void)
{
  freecontext_e2k (&ucp);
}
#endif /* __e2k__  */


void
cf (int i, int j)
{
  if (i != 78 || j != 274)
    {
      printf ("i %d j %d\n", i, j);
      exit (1);
    }
  else if (TEST_STACK_ALIGN ())
    {
      puts ("insufficiently aligned stack");
      exit (2);
    }
}

int
do_test (void)
{
  for (size_t j = 32; j < 64; j += sizeof (long))
    {
      if (getcontext (&ucp) != 0)
	{
	  if (errno == ENOSYS)
	    {
	      puts ("context handling not supported");
	      return 0;
	    }

	  puts ("getcontext failed");
	  return 1;
	}
      ucp.uc_link = &ucp2;
      ucp.uc_stack.ss_sp = st1;
      ucp.uc_stack.ss_size = sizeof (st1) - j;
      memset (&st1[sizeof (st1) - j], 0x55, j);
#ifndef __e2k__
      makecontext (&ucp, (void (*) (void)) cf, 2, 78, 274);
#else /* __e2k__  */
  /* Take care of freeing this context. Neither can we do this within
     `cf ()', nor in `main ()' (we don't return to the latter). Therefore, try
     using atexit () handler.  */
      if (makecontext_e2k (&ucp, (void (*) (void)) cf, 2, 78, 274) != 0)
        {
          printf ("%s: makecontext_e2k: %m\n", __FUNCTION__);
          exit (1);
        }

      atexit (free_ucp);
#endif /* __e2k__  */
      if (swapcontext (&ucp2, &ucp) != 0)
	{
	  puts ("setcontext failed");
	  return 1;
	}

      for (size_t i = j; i > 0; i--)
	if (st1[sizeof (st1) - j + i - 1] != 0x55)
	  { printf ("fail %zd %zd\n", i, j); break; }
    }

  return 0;
}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
