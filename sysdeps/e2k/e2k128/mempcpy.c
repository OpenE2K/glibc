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
