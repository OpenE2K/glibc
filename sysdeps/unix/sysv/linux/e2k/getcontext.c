#include <signal.h>
#include <sysdep.h>
/* FIXME: a compile error is going to happen if HAVE_GETCONTEXT_VSYSCALL hasn't
   been defined in `sysdep.h' since we actually don't have any alternative for
   fast syscall here. Shouldn't I implement a stub version of getcontext ()
   instead? On the other hand, upstream glibc should never be compiled with
   ancient Kernel headers . . .  */
#ifdef HAVE_GETCONTEXT_VSYSCALL
# define HAVE_VSYSCALL
#endif /* HAVE_GETCONTEXT_VSYSCALL  */
#include <sysdep-vdso.h>



int
__getcontext (ucontext_t *ucp)
{
  asm ("rrd %%cr0.hi, %0" : "=r" (ucp->uc_mcontext.cr0_hi));
  asm ("rrd %%cr1.lo, %0" : "=r" (ucp->uc_mcontext.cr1_lo));
  asm ("rrd %%cr1.hi, %0" : "=r" (ucp->uc_mcontext.cr1_hi));

  return INLINE_VSYSCALL_ONLY (getcontext, 2, ucp, _NSIG / 8);
}

weak_alias (__getcontext, getcontext)
