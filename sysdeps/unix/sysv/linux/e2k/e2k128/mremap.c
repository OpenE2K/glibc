#include <sys/types.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <sysdep.h>

void *
__mremap (void *addr, size_t old_len, size_t new_len, int flags, ...)
{
  void *new_addr = NULL;
  if (flags & MREMAP_FIXED)
    {
      va_list arg;
      va_start (arg, flags);
      /* This argument shoudn't make sense in PM in fact.  */
      new_addr = va_arg (arg, void *);
      va_end (arg);
    }

  return INLINE_BOGUS_SYSCALL (mremap, 5, addr, old_len, new_len, flags,
			       new_addr);
}

libc_hidden_def (__mremap)
weak_alias (__mremap, mremap)
