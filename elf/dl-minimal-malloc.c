/* Minimal malloc implementation for dynamic linker and static
   initialization.
   Copyright (C) 1995-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* Mark symbols hidden in static PIE for early self relocation to work.
    Note: string.h may have ifuncs which cannot be hidden on i686.  */
#if BUILD_PIE_DEFAULT
# pragma GCC visibility push(hidden)
#endif
#include <assert.h>
#include <string.h>
#include <ldsodefs.h>
#include <malloc/malloc-internal.h>

#if ! (defined __e2k__ && defined __ptr128__)

static void *alloc_ptr, *alloc_end, *alloc_last_block;

/* Allocate an aligned memory block.  */
void *
__minimal_malloc (size_t n)
{
  if (alloc_end == 0)
    {
      /* Consume any unused space in the last page of our data segment.  */
      extern int _end attribute_hidden;
      alloc_ptr = &_end;
      alloc_end = (void *) 0 + (((alloc_ptr - (void *) 0)
				 + GLRO(dl_pagesize) - 1)
				& ~(GLRO(dl_pagesize) - 1));
    }

  /* Make sure the allocation pointer is ideally aligned.  */
  alloc_ptr = (void *) 0 + (((alloc_ptr - (void *) 0) + MALLOC_ALIGNMENT - 1)
			    & ~(MALLOC_ALIGNMENT - 1));

  if (alloc_ptr + n >= alloc_end || n >= -(uintptr_t) alloc_ptr)
    {
      /* Insufficient space left; allocate another page plus one extra
	 page to reduce number of mmap calls.  */
      caddr_t page;
      size_t nup = (n + GLRO(dl_pagesize) - 1) & ~(GLRO(dl_pagesize) - 1);
      if (__glibc_unlikely (nup == 0 && n != 0))
	return NULL;
      nup += GLRO(dl_pagesize);
      page = __mmap (0, nup, PROT_READ|PROT_WRITE,
		     MAP_ANON|MAP_PRIVATE, -1, 0);
      if (page == MAP_FAILED)
	return NULL;
      if (page != alloc_end)
	alloc_ptr = page;
      alloc_end = page + nup;
    }

  alloc_last_block = (void *) alloc_ptr;
  alloc_ptr += n;
  return alloc_last_block;
}

/* We use this function occasionally since the real implementation may
   be optimized when it can assume the memory it returns already is
   set to NUL.  */
void *
__minimal_calloc (size_t nmemb, size_t size)
{
  /* New memory from the trivial malloc above is always already cleared.
     (We make sure that's true in the rare occasion it might not be,
     by clearing memory in free, below.)  */
  size_t bytes = nmemb * size;

#define HALF_SIZE_T (((size_t) 1) << (8 * sizeof (size_t) / 2))
  if (__builtin_expect ((nmemb | size) >= HALF_SIZE_T, 0)
      && size != 0 && bytes / size != nmemb)
    return NULL;

  return malloc (bytes);
}

/* This will rarely be called.  */
void
__minimal_free (void *ptr)
{
  /* We can free only the last block allocated.  */
  if (ptr == alloc_last_block)
    {
      /* Since this is rare, we clear the freed block here
	 so that calloc can presume malloc returns cleared memory.  */
      memset (alloc_last_block, '\0', alloc_ptr - alloc_last_block);
      alloc_ptr = alloc_last_block;
    }
}

/* This is only called with the most recent block returned by malloc.  */
void *
__minimal_realloc (void *ptr, size_t n)
{
  if (ptr == NULL)
    return malloc (n);
  assert (ptr == alloc_last_block);
  size_t old_size = alloc_ptr - alloc_last_block;
  alloc_ptr = alloc_last_block;
  void *new = malloc (n);
  return new != ptr ? memcpy (new, ptr, old_size) : new;
}

#else /* defined __e2k__ && defined __ptr128__  */

static unsigned int
get_code_base (void)
{
  register unsigned long cud;
  asm volatile ("rrd %%cud.lo, %0" : "=r" (cud));
  return (unsigned int) (cud & 0xffffffff);
}

void *
__minimal_malloc (size_t size)
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
__minimal_calloc (size_t count, size_t size)
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


void
__minimal_free (void *ptr)
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

void *
__minimal_realloc (void *ptr, size_t size)
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

#endif /* defined __e2k__ && defined __ptr128__  */
