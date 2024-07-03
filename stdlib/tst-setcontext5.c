/* Check multiple setcontext calls.
   Copyright (C) 2018-2022 Free Software Foundation, Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

static ucontext_t ctx[2];
static volatile int done;

static void f2 (void);

static void
__attribute__ ((noinline, noclone))
f1 (void)
{
  printf ("start f1\n");
  f2 ();
}

static void
__attribute__ ((noinline, noclone))
f2 (void)
{
  printf ("start f2\n");
  if (setcontext (&ctx[1]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
}

static void
f3 (void)
{
  printf ("start f3\n");
  if (done)
    exit (EXIT_SUCCESS);
  done = 1;
  if (setcontext (&ctx[0]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
}

static int
do_test (void)
{
  char st1[32768];

  puts ("making contexts");
  if (getcontext (&ctx[0]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
#if defined __e2k__
  /* We may presumably "free" context obtained from the previous invocation
     of makecontext_e2k (), but there's probably nowhere to free the last
     context obtained this way as the associated f3 () eventually
     `exit ()'s.  */
  if (ctx[1].uc_link == &ctx[0])
    freecontext_e2k (&ctx[1]);
#endif /* defined __e2k__  */
  if (getcontext (&ctx[1]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  ctx[1].uc_stack.ss_sp = st1;
  ctx[1].uc_stack.ss_size = sizeof st1;
  ctx[1].uc_link = &ctx[0];

#if defined __e2k__
  if (makecontext_e2k
#else /* ! defined __e2k__  */
      makecontext
#endif /* ! defined __e2k__  */
      (&ctx[1], (void (*) (void)) f3, 0)
#if defined __e2k__
      != 0)
    {
      printf ("%s: makecontext_e2k returned non-zero: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
#else /* ! defined __e2k__  */
   ;
#endif /* ! defined __e2k__  */

  f1 ();
  puts ("FAIL: returned from f1 ()");
  exit (EXIT_FAILURE);
}

#include <support/test-driver.c>
