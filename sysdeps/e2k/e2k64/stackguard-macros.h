#include <stdint.h>

#define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("ldd %%dg13, -8, %0" : "=r" (x)); x; })

#define POINTER_CHK_GUARD \
  ({ uintptr_t x; asm ("ldd %%dg13, -16, %0" : "=r" (x)); x; })
