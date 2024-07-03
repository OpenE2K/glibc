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

#include <sys/types.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <sysdep.h>

void *
__mremap (void *addr, size_t old_len, size_t new_len, int flags, ...)
{
  void *new_addr = NULL;
  if (flags & MREMAP_FIXED)
    {
      va_list arg;
      va_start (arg, flags);
      /* This argument shoudn't make sense in PM in fact.  */
      new_addr = va_arg (arg, void *);
      va_end (arg);
    }

  return INLINE_BOGUS_SYSCALL (mremap, 5, addr, old_len, new_len, flags,
			       new_addr);
}

libc_hidden_def (__mremap)
weak_alias (__mremap, mremap)
