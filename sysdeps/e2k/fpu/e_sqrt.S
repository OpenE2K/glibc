/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include <sysdep.h>

ENTRY(__ieee754_sqrt)

#if defined __ptr128__
	setwd	wsz = 0x8
#else /* ! defined __ptr128__  */
	setwd	wsz = 0x4
#endif /* ! defined __ptr128__  */

#ifndef __ptr128__
	fsqrtid	%dr0, %dr1
	fsqrttd	%dr0, %dr1, %dr0
#else /* __ptr128__  */
	fsqrtid	%dr2, %dr1
	fsqrttd	%dr2, %dr1, %dr0
#endif /* __ptr128__  */
	ret

END(__ieee754_sqrt)
strong_alias (__ieee754_sqrt, __sqrt_finite)
