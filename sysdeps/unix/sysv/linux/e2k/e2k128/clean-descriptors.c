#if defined __PROTECTED__
#include <sys/syscall.h>
#if defined __NR_clean_descriptors

#define __NR___syscall_clean_descriptors __NR_clean_descriptors
//static __inline__ _syscall3(int, __syscall_clean_descriptors,
//			void *, ptr, long, size, unsigned long, flags);

static int clean_descriptors(void * ptr, long size, unsigned long flags)
{
  return  INLINE_SYSCALL (clean_descriptors, 3, ptr, size, flags);
}
#else
# error "__NR_clean_descriptors not defined"
#endif
#else /* __PROTECTED__ */
#error "clean_descriptors syscall/function is used only for e2k protected mode"
#endif /* __PROTECTED__ */
