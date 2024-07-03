/* Copyright (C) 1996-2022 Free Software Foundation, Inc.
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

#include <netdb.h>
#include <tls.h>


/* When threaded, h_errno may be a per-thread variable.  */
int *
weak_const_function
#if ! (defined __e2k__ && defined SHARED)
__h_errno_location
#else /* defined __e2k__ && defined SHARED  */
__h_errno_location_local
#endif /* defined __e2k__ && defined SHARED  */
(void)
{
  return &h_errno;
}

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (__h_errno_location)

#else /* defined __e2k__ && defined SHARED  */

/* Create __GI___h_errno_location normally output with the function's body.
   When doing so, preserve its WEAKness specified by "weak_const_function". */
weak_alias (__h_errno_location_local, __h_errno_location)
 
/* Finally, output __h_errno_location@{@GLIBC_2.2,GLIBC_2.0} dynamic symbols
   the latter of which is required for runtime compatibility with
   libpthread-2.29.so. Both of them should be GLOBAL and libc_hidden_def ()
   gets rid of `__GI___h_errno_location's WEAKness thanks to `__attribute__
   ((alias))' in it. Because `__asm__ (".symver ...")' does NOT alter
   WEAKness (as well as HIDDENness) of the created versioned symbol itself,
   take care of this by creating an intermediate strong alias.  */

strong_alias (__h_errno_location_local, __h_errno_location_strong)
versioned_symbol (libc, __h_errno_location_strong, __h_errno_location, GLIBC_2_0);
compat_symbol (libpthread, __h_errno_location_strong, __h_errno_location, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
