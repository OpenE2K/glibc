#include <signal.h>
#include <sysdep.h>

int
__makecontext_e2k (ucontext_t *ucp, void (*func) (void), int argc, ...)
{
  return INLINE_SYSCALL (makecontext, 5, ucp, func,
#if ! defined __ptr128__
			 8 * argc, (unsigned long) &argc + 8,
#else /* defined __ptr128__  */
			 /* Evaluate the size of the parameters' area and the
			    pointer to the first argument in `...' according
			    to the new PM ABI. When calculating the latter
			    take into account that `sizeof (argc) == 4'.  */
			 16UL * argc, &argc + 4,
#endif /* defined __ptr128__  */
			 _NSIG / 8);
}

weak_alias (__makecontext_e2k, makecontext_e2k)
