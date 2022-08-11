/* Definitions for POSIX memory map interface.  Linux/E2K version.
   Copyright (C) 1997-2020 Free Software Foundation, Inc.
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

#ifndef _SYS_MMAN_H
# error "Never use <bits/mman.h> directly; include <sys/mman.h> instead."
#endif

/* The following definitions basically come from the kernel headers.
   But the kernel header is not namespace clean.  */

# define __MAP_ANONYMOUS	0x10	/* Don't use a file.  */

/* These are Linux-specific.  */
#ifdef __USE_MISC
# define MAP_DENYWRITE	0x0800		/* ETXTBSY */
# define MAP_GROWSDOWN	0x1000		/* Stack-like segment.  */
# define MAP_EXECUTABLE	0x4000		/* Mark it as an executable.  */
# define MAP_LOCKED	0x8000		/* Lock the mapping.  */
# define MAP_NORESERVE	0x10000		/* Don't check for reservations.  */
# define MAP_POPULATE	0x20000		/* Populate (prefault) pagetables.  */
# define MAP_NONBLOCK	0x40000		/* Do not block on IO.  */
# define MAP_HUGETLB	0x200000	/* Create huge page mapping.  */
# define MAP_FIXED_NOREPLACE 0x400000	/* MAP_FIXED but do not unmap
					   underlying mapping */
# define MAP_STACK	0x0             /* Not supported in the Kernel yet.  */
#endif

/* Flags to `msync'.  */
#define MS_ASYNC	1		/* Sync memory asynchronously.  */
#define MS_SYNC		4		/* Synchronous memory sync.  */
#define MS_INVALIDATE	2		/* Invalidate the caches.  */

/* Flags for `mlockall'.  */
#define MCL_CURRENT	1		/* Lock all currently mapped pages.  */
#define MCL_FUTURE	2		/* Lock all additions to address
					   space.  */

/* Include generic Linux declarations.  */
#include <bits/mman-linux.h>

/* Other flags.  */
#ifdef __USE_MISC
# define MAP_RENAME	MAP_ANONYMOUS
#endif

/* Values that differ from standard <mman-linux.h>.  */

#undef PROT_GROWSDOWN
#define PROT_GROWSDOWN	0x20		/* Extend change to start of
					   growsdown vma (mprotect only).  */

#undef PROT_GROWSUP
#define PROT_GROWSUP	0x40		/* Extend change to start of
					   growsup vma (mprotect only).  */

#undef MAP_FIXED
#define MAP_FIXED	0x100		/* Interpret addr exactly.  */
