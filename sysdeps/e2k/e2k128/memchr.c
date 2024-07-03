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

#ifndef _LIBC
# include <config.h>
#endif

#include <string.h>

#include <stddef.h>

#include <limits.h>

#undef __memchr
#ifdef _LIBC
# undef memchr
#endif

#ifndef weak_alias
# define __memchr memchr
#endif

#ifndef MEMCHR
# define MEMCHR __memchr
#endif

/* Search no more than N bytes of S for C.  */
void *
MEMCHR (void const *s, int c_in, size_t n)
{
  const unsigned char *src = (const unsigned char *) s;
  unsigned char c = (unsigned char) c_in;

  while (n--) 
    {
      if (*src == c)
	return (char *) src;
      src++;
    }

  return NULL;
}

#ifdef weak_alias
weak_alias (__memchr, memchr)
#endif
libc_hidden_builtin_def (memchr)
