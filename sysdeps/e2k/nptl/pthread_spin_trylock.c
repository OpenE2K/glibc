#include <errno.h>
#include "pthreadP.h"

#define STW __p2(__STW)
#define LDW __p2(__LDW)

int
pthread_spin_trylock (pthread_spinlock_t *lock)
{
  register int tmp, locked = SPIN_LOCK_LOCKED_VAL;
  asm volatile
    ("1:\n" LDW ",0 %1, 0x0, %0, mas = 0x7\n"
     "   {\n"
     "   " STW ",2 %1, 0x0, %2, mas = 0x2\n"
     "   ibranch 1b ? %%MLOCK\n"
     "   }\n"
     "   wait    all_c = 1, fl_c = 1"
     : "=&r" (tmp)
     : "r" (lock), "r" (locked)
     : "memory");
  return tmp != locked ? 0 : EBUSY;
}

