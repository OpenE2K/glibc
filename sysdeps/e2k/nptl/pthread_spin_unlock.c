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

#include "pthreadP.h"

#define STW __p2(__STW)
int pthread_spin_unlock (pthread_spinlock_t *lock)
{
  register int tmp = SPIN_LOCK_UNLOCKED_VAL;

  /* Exotic operations like WAIT are no longer supported in asm_inline `asm's
     (see Bug #69048).  */
#pragma no_asm_inline
  asm volatile
    ("wait    all_c = 1, fl_c = 1\n"
     STW ",2 %0, 0x0, %1, mas = 0x0"
     :
     : "r" (lock), "r" (tmp)
     : "memory");
  return 0;
}

