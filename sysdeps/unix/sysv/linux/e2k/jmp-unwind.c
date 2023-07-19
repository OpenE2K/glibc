#include <setjmp.h>

void
_longjmp_unwind (jmp_buf env, int val)
{
  /*  __pthread_cleanup_upto (env->__jmpbuf, CURRENT_STACK_FRAME) is NOT
      workable on e2k yet. Neither in PM, nor in ordinary modes.  */
}
