/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/* Copy memory to memory until the specified number of bytes
   has been copied, return pointer to following byte.
   Overlap is NOT handled correctly. */

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
