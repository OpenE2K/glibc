/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Thread-local storage handling in the ELF dynamic linker.  E2K version.
   Copyright (C) 2003, 2005, 2006 Free Software Foundation, Inc.
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

#include <csu/libc-tls.c>

/* On E2K, as it lacks linker optimizations, __tls_get_addr can be
   called even in statically linked binaries.
   In this case module must be always 1 and PT_TLS segment
   exist in the binary, otherwise it would not link.  */

void *
__tls_get_addr (size_t m, size_t offset)
{
  dtv_t *dtv = THREAD_DTV ();
  return (char *) dtv[1].pointer.val + offset;
}
