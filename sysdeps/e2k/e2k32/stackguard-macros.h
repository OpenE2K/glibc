#include <stdint.h>

#define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("ldgdw %%g13, -4, %0" : "=r" (x)); x; })

#define POINTER_CHK_GUARD \
  ({ uintptr_t x; asm ("ldgdw %%g13, -8, %0" : "=r" (x)); x; })
