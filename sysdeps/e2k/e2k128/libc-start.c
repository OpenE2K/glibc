#ifndef SHARED

/* Mark symbols hidden in static PIE for early self relocation to work.  */
# if BUILD_PIE_DEFAULT
#  pragma GCC visibility push(hidden)
# endif
# include <ldsodefs.h>
# include <cpu-features.c>

# define ARCH_INIT_CPU_FEATURES() init_cpu_features ()

#endif /* ! defined SHARED  */
#include <csu/libc-start.c>
