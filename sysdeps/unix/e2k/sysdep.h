/* Copyright (C) 1993, 1994, 1995, 1997, 2003 Free Software Foundation, Inc.
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

#include <sysdeps/unix/sysdep.h>

#ifdef  __ASSEMBLER__

#define ENTRY(name)			\
	.global	C_SYMBOL_NAME(name);	\
	.type	name, @function;	\
	.align	8;			\
C_LABEL(name)

#undef PSEUDO_END
#define PSEUDO_END(name) \
	.size name, .-name

#undef END
#define END(name) \
	.size name, .-name

#define ret \
	return	%ctpr3; \
	ct	%ctpr3;

#undef PSEUDO_END_NOERRNO
#define PSEUDO_END_NOERRNO(name) PSEUDO_END(name)

#undef ret_NOERRNO
#define ret_NOERRNO ret

#endif  /* __ASSEMBLER__ */
