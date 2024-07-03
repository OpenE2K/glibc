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

#include <string.h>

#undef strrchr

#ifndef STRRCHR
# define STRRCHR strrchr
#endif


char *
STRRCHR (const char *s, int i)
{
  const char *last = NULL;
  char c = i;

  while (*s)
    {
      if (*s == c)
	{
	  last = s;
	}

      s++;
    }

  if (*s == c)
    {
      last = s;
    }


  return (char *) last;
}

#ifdef weak_alias
# undef rindex
weak_alias (strrchr, rindex)
#endif
libc_hidden_builtin_def (strrchr)
