/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Copy memory to memory until the specified number of bytes
   has been copied, return pointer to following byte.
   Overlap is NOT handled correctly.
   Copyright (C) 1991, 1997, 1998, 2002, 2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Torbjorn Granlund (tege@sics.se).

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

#define NO_MEMPCPY_STPCPY_REDIRECT
#include <string.h>

#undef memcpy
#undef mempcpy
#undef __mempcpy
#define	memcpy	__mempcpy
#define	RETURN(p,shift)	return (void*)((char*)p + shift)

#include <memcpy.c>

#if 0
libc_hidden_def (__mempcpy)
#endif /* 0  */
weak_alias (__mempcpy, mempcpy)
libc_hidden_builtin_def (mempcpy)
