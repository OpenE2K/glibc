/* Check setcontext on the context from makecontext.
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
#include <stdatomic.h>

/* do_test () --> via swapcontext (ctx[1]) --> f1a () --> via call --> f2 ()
   --> via swapcontext (ctx[2]) --> f1b () --> via setcontext (ctx[3])
   --> do_test () --> via set_context (ctx[4]) --> f2 ()

   ctx[1] and ctx[2] are obtained from5 makecontext ()

   This way it should be possible to free ctx[2], but NOT ctx[1] in the end
   (i.e. just before exit (EXIT_SUCCESS) from f2 ()).  */

static ucontext_t ctx[5];
static atomic_int done;

static void
__attribute__((noinline, noclone))
f2 (void)
{
  done++;
  puts ("swap contexts in f2");
  if (swapcontext (&ctx[4], &ctx[2]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
#if defined __e2k__
  /* ctx[2] can probably be freed as execution should no longer return to it.
     There is no obvious way to free ctx[1], though, as we are still executing
     on it.  */
  freecontext_e2k (&ctx[2]);
#endif /* defined __e2k__  */
  puts ("end f2");
  exit (done == 2 ? EXIT_SUCCESS : EXIT_FAILURE);
}

static void
f1b (void)
{
  if (done)
    {
      puts ("set context in f1b");
      if (setcontext (&ctx[3]) != 0)
	{
	  printf ("%s: setcontext: %m\n", __FUNCTION__);
	  exit (EXIT_FAILURE);
	}
    }
  exit (EXIT_FAILURE);
}

static void
f1a (void)
{
  static char st2[32768];
  puts ("start f1a");
  if (getcontext (&ctx[2]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  ctx[2].uc_stack.ss_sp = st2;
  ctx[2].uc_stack.ss_size = sizeof st2;
  ctx[2].uc_link = &ctx[0];

#if defined __e2k__
  /* No idea where it could be possible to free ctx[1] as the test should
     normally `exit (EXIT_SUCCESS)' from the associated f1 ().  */
  if (makecontext_e2k
#else /* ! defined __e2k__  */
      makecontext
#endif /* ! defined __e2k__  */
      (&ctx[2], (void (*) (void)) f1b, 0)
#if defined __e2k__
      != 0)
    {
      printf ("%s: makecontext_e2k returned non-zero: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
#else /* ! defined __e2k__  */
   ;
#endif /* ! defined __e2k__  */
  
  f2 ();
}

/* The execution path through the test looks like this:
   do_test (call)
   -> "making contexts"
   -> "swap contexts"
   f1a (via swapcontext to ctx[1], with alternate stack)
   -> "start f1a"
   f2 (call)
   -> "swap contexts in f2"
   f1b (via swapcontext to ctx[2], with alternate stack)
   -> "set context in f1b"
   do_test (via setcontext to ctx[3], main stack)
   -> "setcontext"
   f2 (via setcontext to ctx[4], with alternate stack)
   -> "end f2"

   We must use an alternate stack for f1b, because if we don't then the
   result of executing an earlier caller may overwrite registers
   spilled to the stack in f2.  */
static int
do_test (void)
{
  static char st1[32768];
  puts ("making contexts");
  if (getcontext (&ctx[0]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  if (getcontext (&ctx[1]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  ctx[1].uc_stack.ss_sp = st1;
  ctx[1].uc_stack.ss_size = sizeof st1;
  ctx[1].uc_link = &ctx[0];

#if defined __e2k__
  /* No idea where it could be possible to free ctx[1] as the test should
     normally `exit (EXIT_SUCCESS)' from the associated f1 ().  */
  if (makecontext_e2k
#else /* ! defined __e2k__  */
      makecontext
#endif /* ! defined __e2k__  */
      (&ctx[1], (void (*) (void)) f1a, 0)
#if defined __e2k__
      != 0)
    {
      printf ("%s: makecontext_e2k returned non-zero: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
#else /* ! defined __e2k__  */
   ;
#endif /* ! defined __e2k__  */

  
  puts ("swap contexts");
  if (swapcontext (&ctx[3], &ctx[1]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  if (done != 1)
    exit (EXIT_FAILURE);
  done++;
  puts ("set context");
  if (setcontext (&ctx[4]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  exit (EXIT_FAILURE);
}

#include <support/test-driver.c>
