#include <signal.h>
#include <sysdep.h>

void
__freecontext_e2k (ucontext_t *ucp)
{
  INLINE_SYSCALL (freecontext, 1, ucp);
}

weak_alias (__freecontext_e2k, freecontext_e2k)
