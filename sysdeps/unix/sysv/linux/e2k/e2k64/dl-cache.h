/* Support for reading /etc/ld.so.cache files written by Linux ldconfig.
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.
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

/* ATTENTION!!! This macro should coincide with (FLAG_ELF_LIBC6 |
   FLAG_E2K_LIB64), otherwise ld.so won't be consistent with ldconfig.
   It's not quite clear why this requirement can't be encoded in glibc
   sources explicitly. */
#define _DL_CACHE_DEFAULT_ID	0x803

#define _dl_cache_check_flags(flags)			\
  ((flags) == _DL_CACHE_DEFAULT_ID)

#include_next <dl-cache.h>
