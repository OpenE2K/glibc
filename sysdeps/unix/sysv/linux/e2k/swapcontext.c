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
  if (oucp)
    {
      asm ("rrd %%cr0.hi, %0" : "=r" (oucp->uc_mcontext.cr0_hi));
      asm ("rrd %%cr1.lo, %0" : "=r" (oucp->uc_mcontext.cr1_lo));
      asm ("rrd %%cr1.hi, %0" : "=r" (oucp->uc_mcontext.cr1_hi));
    }

  return INLINE_SYSCALL (swapcontext, 3, oucp, ucp, _NSIG / 8);
}

weak_alias (__swapcontext, swapcontext)
