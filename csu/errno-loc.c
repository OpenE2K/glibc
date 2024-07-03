/* MT support function to get address of `errno' variable, non-threaded
   version.
   Copyright (C) 1996-2022 Free Software Foundation, Inc.
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
#include <tls.h>

int *
#if ! (defined __e2k__ && defined SHARED && ! IS_IN (rtld))
__errno_location
#else /* defined __e2k__  */
__errno_location_local
#endif /* defined __e2k__  */
(void)
{
  return &errno;
}

#if ! (defined __e2k__ && defined SHARED && ! IS_IN (rtld))

libc_hidden_def (__errno_location)

#else /* defined __e2k__ && defined SHARED && ! IS_IN (rtld) */

strong_alias (__errno_location_local, __errno_location)

#include <shlib-compat.h>

versioned_symbol (libc, __errno_location_local, __errno_location, GLIBC_2_0);
compat_symbol (libpthread, __errno_location_local, __errno_location, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
