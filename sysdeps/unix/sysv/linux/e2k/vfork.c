#if defined __ptr128__ && defined LIBC_NONSHARED
/* Now that in PM `vfork ()' is placed into libc_nonshared.a rather than in
   libc.so ensure that `errno' is referenced as `errno' from the below `INLINE
   _SYSCALL' not as `__libc_errno' locally defined in libc.so.  */
# define __libc_errno errno
#endif

#include <stdlib.h>
#include <tls.h>
#include <unistd.h>

__pid_t
__vfork (void)
{
  int res;

  /* vfork () isn't implemented in the Kernel.  */
  if ((__libc_e2k_system_info & 0x1) == 0)
    return __fork ();

  res = INLINE_SYSCALL (vfork, 0);
  return res;
}

libc_hidden_def (__vfork)

weak_alias (__vfork, vfork)
