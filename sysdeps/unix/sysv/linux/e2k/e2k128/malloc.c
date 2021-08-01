#if !IS_IN (rtld)

/* This include replaces the original `#include "malloc.h"' in `*-uclibc.c'
   files included below.  */
#include "malloc-uclibc.h"

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


#else /* IS_IN (rtld)   */

#include <stdlib.h>
#include <sys/mman.h>
#include <malloc/malloc-internal.h>


static unsigned int
get_code_base (void)
{
  register unsigned long cud;
  asm volatile ("rrd %%cud.lo, %0" : "=r" (cud));
  return (unsigned int) (cud & 0xffffffff);
}

void *
malloc (size_t size)
{
  void *res;

  if (size == 0)
    size = 1;

  if (get_code_base () == 0)
    /* Use this bogus unreliable syscall only on LSIM for the sake of
       Bug #105333 as it may very well halt a real host. To distinguish
       between LSIM and the real host make use of the fact that `%cud.base
       == 0' only on the former.  */
    res = INLINE_BOGUS_SYSCALL (get_mem, 1, size);
  else
    {
      size = (size + 0xfffUL) & ~0xfffUL;
      res = mmap (NULL, size, PROT_READ | PROT_WRITE,
		  MAP_ANON|MAP_PRIVATE, -1, 0);
    }

  /* malloc () is expected to return NULL on failure rather than -1.  */
  if (((((long) res) << 16) >> 16) == -1L)
    res = NULL;

  return res;
}

void *
calloc (size_t count, size_t size)
{
  size_t req = 0;

  if (count != 0)
    {
      req = count * size;
      if (((count | size) & ~(size_t)0xffff) &&
	  (req / count != size))
	req = (size_t) -1; /* force downstream failure on overflow */
    }

  return memset (malloc (req), 0, req);
}

void *
realloc (void *ptr, size_t size)
{
  if (ptr == NULL)
    return malloc (size);
  else
    {
      size_t oldsize = __builtin_e2k_get_ap_size(ptr);

      if (oldsize >= size && oldsize < size * 2)
	return ptr;
      else
        {
	  void* newmem = malloc (size);
	  memcpy (newmem, ptr, oldsize < size ? oldsize : size);
	  free (ptr);
	  return newmem;
        }
    }
}

void
free (void *ptr)
{
  /* malloc () currently returns APs with `curptr == 0'. Therefore, it should
     be the user error if she attempts to free something not matching this
     condition. TODO: doesn't it make sense to generate a signal in this
     case?
     FIXME: what about pointers with alignments greater than that of a page
     returned by memalign ()? It should be possible to free () them. On
     the other hand such alignments should make no sense especially in PM.  */
  if (__builtin_e2k_get_ap_curptr (ptr) != 0)
    return;

  if (get_code_base () == 0)
    {
      if (ptr != NULL)
	INLINE_SYSCALL (free_mem, 1, ptr);
    }
  else
    munmap (ptr, __builtin_e2k_get_ap_size (ptr));
}

#endif /* IS_IN (rtld)  */

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
