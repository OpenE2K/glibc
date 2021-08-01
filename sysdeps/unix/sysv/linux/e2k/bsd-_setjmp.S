#include <sysdep.h>

#undef  NEED_MASK
#undef  HAS_SECOND_ARG
#undef  FUNCNAME
#define FUNCNAME _setjmp

#include "setjmp_tmpl.S"

weak_extern (_setjmp)
libc_hidden_def (_setjmp)
