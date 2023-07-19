/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* This file is part of the GNU C Library.

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

#include <sysdep.h>

ENTRY(__ieee754_sqrtl)

#if defined __ptr128__
	setwd	wsz = 0x8
#else /* ! defined __ptr128__  */
	setwd	wsz = 0x4
#endif /* ! defined __ptr128__  */

#ifndef __ptr128__
	movif     %dr0, %r1, %dr0
#else /* __ptr128__  */
	movif     %dr2, %r3, %dr0
#endif /* __ptr128__  */
	fxsqrtixx %dr0, %dr1
	fxsqrtuxx %dr0, %dr1, %dr1
	fxsqrttxx %dr0, %dr1, %dr0
	movfi     %dr0, %r1
	sxt    5, %r1, %dr1
	ret

END(__ieee754_sqrtl)
strong_alias (__ieee754_sqrtl, __sqrtl_finite)
