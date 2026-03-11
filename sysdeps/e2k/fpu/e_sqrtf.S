/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <sysdep.h>

ENTRY(__ieee754_sqrtf)

#if defined __ptr128__
	setwd	wsz = 0x8
#else /* ! defined __ptr128__  */
	setwd	wsz = 0x4
#endif /* ! defined __ptr128__  */

#ifndef __ptr128__
	fsqrts	%dr0, %dr0
#else /* __ptr128__ */
	fsqrts	%dr2, %dr0
#endif /* __ptr128__ */
	ret

END(__ieee754_sqrtf)
strong_alias (__ieee754_sqrtf, __sqrtf_finite)
