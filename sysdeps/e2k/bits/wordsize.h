/* Copyright (C) 1999 Free Software Foundation, Inc.
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

#if defined __ptr64__
# define __WORDSIZE			64
#elif defined __ptr32__
# define __WORDSIZE			32
# define __WORDSIZE32_SIZE_ULONG	0
# define __WORDSIZE32_PTRDIFF_LONG	0
#elif defined __ptr128__
# define __WORDSIZE			64
#else
# error "not supported pointer type"
#endif

/* We've implicitly had this setting for years due to the absence of this
   macro. Leave it this way for the sake of `struct {lastlog,utmp{,x}}'
   backward compatibility.  */
#define __WORDSIZE_TIME64_COMPAT32	0
