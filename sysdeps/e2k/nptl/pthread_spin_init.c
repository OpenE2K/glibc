#include "pthreadP.h"

int
pthread_spin_init (pthread_spinlock_t *lock, int pshared)
{
  /* We can ignore the `pshared' parameter.  Since we are busy-waiting
     all processes which can access the memory location `lock' points
     to can use the spinlock.  */
  *lock = SPIN_LOCK_UNLOCKED_VAL;
  return 0;
}
