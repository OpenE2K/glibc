#include <elf/dl-tunables.h>

static inline void
init_cpu_features (void)
{
#if __iset__ >= 7 && ! defined __elbrus_maket32c__
  u_int64_t mode_ld = TUNABLE_GET (glibc, cpu, madmr_mode_ld, u_int64_t, NULL);
  u_int64_t mode_st = TUNABLE_GET (glibc, cpu, madmr_mode_st, u_int64_t, NULL);

  register u_int64_t madmr = (mode_st << 2) | mode_ld;
  _Pragma ("no_asm_inline")
    __asm__ ("mmurw %0, %%madmr"  : : "r" (madmr));
#endif /* __iset__ >= 7 && ! defined __elbrus_maket32c__  */


}
