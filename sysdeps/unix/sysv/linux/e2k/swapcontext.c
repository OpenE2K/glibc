#if defined __ptr128__ && defined LIBC_NONSHARED
/* Now that in PM `swapcontext ()' has been moved to libc_nonshared.a from
   libc.so ensure that `errno' is referenced as `errno' from the below `INLINE
   _SYSCALL' not as `__libc_errno' locally defined in libc.so.  */
# define __libc_errno errno
#endif
#include <sysdep.h>

int
__swapcontext (ucontext_t *oucp, const ucontext_t *ucp)
{
  return INLINE_SYSCALL (swapcontext, 3, oucp, ucp, _NSIG / 8);
}

weak_alias (__swapcontext, swapcontext)
