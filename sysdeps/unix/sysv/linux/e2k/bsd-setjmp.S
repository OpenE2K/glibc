#include <sysdep.h>

#define NEED_MASK
#undef  HAS_SECOND_ARG
#undef  FUNCNAME
#define FUNCNAME setjmp

#include "setjmp_tmpl.S"

weak_extern (setjmp)
