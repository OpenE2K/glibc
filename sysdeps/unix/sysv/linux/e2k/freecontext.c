#include <signal.h>
#include <sysdep.h>
#include <ucontext.h>

int
__freecontext_e2k (ucontext_t *ucp)
{
  return INLINE_SYSCALL (freecontext, 1, ucp);
}

weak_alias (__freecontext_e2k, freecontext_e2k)
