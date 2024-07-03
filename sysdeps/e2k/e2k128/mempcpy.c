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
