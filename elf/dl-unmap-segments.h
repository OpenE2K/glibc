/* Unmap a shared object's segments.  Generic version.
   Copyright (C) 2014-2019 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#ifndef _DL_UNMAP_SEGMENTS_H
#define _DL_UNMAP_SEGMENTS_H	1

#include <link.h>
#include <sys/mman.h>

/* _dl_map_segments ensures that any whole pages in gaps between segments
   are filled in with PROT_NONE mappings.  So we can just unmap the whole
   range in one fell swoop.  */

static __always_inline void
_dl_unmap_segments (struct link_map *l)
{
  /* FIXME: do we have any counterpart for `uselib' syscall for releasing
     link_map on the Kernel side?  */
#if ! defined __ptr128__
  __munmap ((void *) l->l_map_start, l->l_map_end - l->l_map_start);
#elif /* defined __ptr128__ &&  */ defined __ptr128_new_abi__
  INLINE_SYSCALL_CALL (unuselib, l->l_gd);
#endif /* defined __ptr128__ && defined __ptr128_new_abi__  */
}

#endif  /* dl-unmap-segments.h */
