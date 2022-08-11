#ifndef __ARCH_FORK_H
#define __ARCH_FORK_H

#include <unistd.h>

/* Call the clone syscall with fork semantic.  The CTID address is used
   to store the child thread ID at its locationm, to erase it in child memory
   when the child exits, and do a wakeup on the futex at that address.

   The architecture with non-default kernel abi semantic should correctlly
   override it with one of the supported calling convention (check generic
   kernel-features.h for the clone abi variants).  */
static inline pid_t
arch_fork (void *ctid)
{
  const int flags = CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID | SIGCHLD;
  long ret;

#if ! defined __ptr128_new_abi__
  void *arr[3];

  arr[0] = NULL;
  arr[1] = ctid;
  arr[2] = NULL;
#endif /* ! defined __ptr128_new_abi__  */
  ret = INLINE_SYSCALL_CALL (clone, flags, (void *) 0,
#if ! defined __ptr128_new_abi__
			     arr
#else /* defined __ptr128_new_abi__  */
			     NULL, ctid, NULL
#endif /* defined __ptr128_new_abi__  */
			     );

  return ret;
}

#endif /* __ARCH_FORK_H  */
