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
#include <memcopy.h>
#include <stdlib.h>

#undef strchr


char *
strchr (const char *s, int i)
{
  char c = i;

  while (*s && *s != c) 
    {
      s++;
    }

  if (*s != c) 
    {
      s = NULL;
    }

  return (char *) s;
}

#ifdef weak_alias
# undef index
weak_alias (strchr, index)
#endif
libc_hidden_builtin_def (strchr)
