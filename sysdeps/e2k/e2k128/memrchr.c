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

#include <stdlib.h>

void *
__memrchr (const void *s, int c_in, size_t n)
{
  const unsigned char *src = (const unsigned char *) s;
  unsigned char c = (unsigned char) c_in;

  while (n--) 
    {
      if (src[n] == c)
	return (void *) &src[n];
    }

  return NULL;
}

weak_alias (__memrchr, memrchr)
