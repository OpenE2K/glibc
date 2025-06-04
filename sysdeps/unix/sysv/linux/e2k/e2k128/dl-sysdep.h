/* This will include `sysdeps/unix/sysv/linux/dl-sysdep.h', . . .  */
#include_next <dl-sysdep.h>

/* . . .  which will set NEED_DL_SYSINFO_DSO irrelevant to PM to 1. Take
   care of undefining it.  */

#undef NEED_DL_SYSINFO_DSO
