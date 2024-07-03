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

#if defined __ptr128__ && defined LIBC_NONSHARED
/* Now that in PM `swapcontext ()' has been moved to libc_nonshared.a from
   libc.so ensure that `errno' is referenced as `errno' from the below `INLINE
   _SYSCALL' not as `__libc_errno' locally defined in libc.so.  */
# define __libc_errno errno
#endif
#include <sysdep.h>

int
__swapcontext (ucontext_t *oucp, const ucontext_t *ucp)
{
  return INLINE_SYSCALL (swapcontext, 3, oucp, ucp, _NSIG / 8);
}

weak_alias (__swapcontext, swapcontext)
