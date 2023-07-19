#include "pthreadP.h"

#define STW __p2(__STW)
#define LDW __p2(__LDW)

/* This implementation is similar to the one used in the Linux kernel.  */
int
pthread_spin_lock (pthread_spinlock_t *lock)
{
  register int tmp, locked = SPIN_LOCK_LOCKED_VAL;
  asm volatile
    ("   disp    %%ctpr1, 3f\n"
     "1:\n" LDW ",0 %1, 0x0, %0, mas = 0x7\n"
     "   {\n"
     "   " STW ",2 %1, 0x0, %2, mas = 0x2\n"
     "   ibranch 1b ? %%MLOCK\n"
     "   }\n"
     "   cmpesb  %0, %2, %%pred0\n"
     "   wait    all_c = 1, fl_c = 1\n"
     "   ct      %%ctpr1 ? ~ %%pred0\n"
     "   disp    %%ctpr2, 2f\n"
     "   disp    %%ctpr3, 1b\n"
     "2:\n" LDW " %1, 0x0, %0\n"
     "   cmpesb  %0, %2, %%pred0\n"
     "   wait    all_c = 1, fl_c = 1\n"
     "   ct      %%ctpr2 ? %%pred0\n"
     "   ct      %%ctpr3\n"
     "3: "
     : "=&r" (tmp)
     : "r" (lock), "r" (locked)
     : "memory", 
       "%ctpr1",
       "%ctpr2",
       "%ctpr3",
       "%pred0");
  return 0;
}
