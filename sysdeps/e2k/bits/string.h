/* Optimized, inlined string functions.  SPARC version.
   Copyright (C) 2000 Free Software Foundation, Inc.
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

#ifndef _STRING_H
# error "Never use <bits/string.h> directly; include <string.h> instead."
#endif

/* Don't inline mempcpy into memcpy as E2K has an optimized mempcpy . . .
   FIXME: I am not really sure if it's the case for E2K. This macro is actually
   required to prevent the substitution of `{,__}mempcpy ()' with `always_inline
   __mempcpy_inline ()' during the build of glibc. This is a problem when
   building glibc with `lcc -O0', since the latter turns out to be incapable of
   inlining `always_inline' functions within GNU Statement Expressions (i.e
   `({ . . .})'). This lead to unresolved references to  `__mempcpy_inline' at
   glibc linkage time. I wonder if this could cause similar problems when
   compiling user programs including <string.h>.  */
#define _HAVE_STRING_ARCH_mempcpy 1
