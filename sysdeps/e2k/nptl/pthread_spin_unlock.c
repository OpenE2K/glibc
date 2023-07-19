#include "pthreadP.h"

#define STW __p2(__STW)
int pthread_spin_unlock (pthread_spinlock_t *lock)
{
  register int tmp = SPIN_LOCK_UNLOCKED_VAL;

  /* Exotic operations like WAIT are no longer supported in asm_inline `asm's
     (see Bug #69048).  */
#pragma no_asm_inline
  asm volatile
    ("wait    all_c = 1, fl_c = 1\n"
     STW ",2 %0, 0x0, %1, mas = 0x0"
     :
     : "r" (lock), "r" (tmp)
     : "memory");
  return 0;
}

