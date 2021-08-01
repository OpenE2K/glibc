#include <sysdep.h>

int
__swapcontext (ucontext_t *oucp, const ucontext_t *ucp)
{
  return INLINE_SYSCALL (swapcontext, 3, oucp, ucp, _NSIG / 8);
}

weak_alias (__swapcontext, swapcontext)
