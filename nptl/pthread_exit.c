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

#include <stdlib.h>
#include "pthreadP.h"
#include <unwind-link.h>
#include <stdio.h>
#include <gnu/lib-names.h>

void
__pthread_exit (void *value)
{
  {
    struct unwind_link *unwind_link = __libc_unwind_link_get ();
    if (unwind_link == NULL)
      __libc_fatal (LIBGCC_S_SO
                    " must be installed for pthread_exit to work\n");
  }

  THREAD_SETMEM (THREAD_SELF, result, value);

  __do_cancel ();
}
libc_hidden_def (__pthread_exit)
#if ! (defined __e2k__ && defined SHARED)
weak_alias (__pthread_exit, pthread_exit)
#else /* defined __e2k__ && defined SHARED  */
# include <shlib-compat.h>
# if OTHER_SHLIB_COMPAT (libpthread, GLIBC_2_0, GLIBC_2_2)
weak_alias (__pthread_exit, __pthread_exit_weak)
/* NOTE: pthread_exit () used to be non-WEAK in libc-2.29.so unlike
   libpthread-2.29.so, but in upstream libc.so it's WEAK.  */
versioned_symbol (libc, __pthread_exit_weak, pthread_exit, GLIBC_2_0);
compat_symbol (libpthread, __pthread_exit_weak, pthread_exit, GLIBC_2_0);
# endif
#endif /* defined __e2k__ && defined SHARED  */

/* Ensure that the unwinder is always linked in (the __pthread_unwind
   reference from __do_cancel is weak).  Use ___pthread_unwind_next
   (three underscores) to produce a strong reference to the same
   file.  */
PTHREAD_STATIC_FN_REQUIRE (___pthread_unwind_next)
