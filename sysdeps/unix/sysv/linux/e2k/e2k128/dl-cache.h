#define _DL_CACHE_DEFAULT_ID	0x903

#define _dl_cache_check_flags(flags)			\
  ((flags) == _DL_CACHE_DEFAULT_ID)

#include_next <dl-cache.h>
