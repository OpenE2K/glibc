/* This include replaces the original `#include "malloc.h"' in `*-uclibc.c'
   files included below.  */
#include "malloc-uclibc.h"

#include "clean-descriptors.c"
#include "calloc-uclibc.c"
#include "free-uclibc.c"
#include "mallinfo-uclibc.c"
#include "malloc-uclibc.c"
#include "mallopt-uclibc.c"
#include "memalign-uclibc.c"
#include "realloc-uclibc.c"

int __libc_malloc_initialized = 1;

void weak_function (*__free_hook) (void *__ptr,
                                   const void *) = NULL;
void *weak_function (*__malloc_hook)
  (size_t __size, const void *) = NULL;
void *weak_function (*__realloc_hook)
  (void *__ptr, size_t __size, const void *)
  = NULL;
void *weak_function (*__memalign_hook)
  (size_t __alignment, size_t __size, const void *)
  = NULL;

void
__malloc_fork_lock_parent (void)
{
  __MALLOC_LOCK;
}

void
__malloc_fork_unlock_parent (void)
{
  __MALLOC_UNLOCK;
}

void
__malloc_fork_unlock_child (void)
{
  __MALLOC_UNLOCK;
}


#if 0
void *
memalign (size_t alignment, size_t size)
{
  unsigned int delta;
  void *ptr = malloc (size + alignment - 1);

  delta = alignment - (unsigned int) ptr % alignment;
  if (delta != alignment)
    ptr += delta;

  return ptr;
}

int
posix_memalign (void **memptr, size_t alignment, size_t size)
{
  *memptr = memalign (alignment, size);
  return 0;
}

#endif /* if 0  */
