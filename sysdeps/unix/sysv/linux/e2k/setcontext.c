#include <signal.h>
#include <sysdep.h>

int
__setcontext (const ucontext_t *ucp)
{
  return INLINE_SYSCALL (setcontext, 2, ucp, _NSIG / 8);
}

weak_alias (__setcontext, setcontext)
