/* Copyright (c) 2009-2024 AO MCST.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
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
