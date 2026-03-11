/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

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
