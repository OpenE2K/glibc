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
#define LDW __p2(__LDW)

/* This implementation is similar to the one used in the Linux kernel.  */
int
pthread_spin_lock (pthread_spinlock_t *lock)
{
  register int tmp, locked = SPIN_LOCK_LOCKED_VAL;
  asm volatile
    ("   disp    %%ctpr1, 3f\n"
     "1:\n" LDW ",0 %1, 0x0, %0, mas = 0x7\n"
     "   {\n"
     "   " STW ",2 %1, 0x0, %2, mas = 0x2\n"
     "   ibranch 1b ? %%MLOCK\n"
     "   }\n"
     "   cmpesb  %0, %2, %%pred0\n"
     "   wait    all_c = 1, fl_c = 1\n"
     "   ct      %%ctpr1 ? ~ %%pred0\n"
     "   disp    %%ctpr2, 2f\n"
     "   disp    %%ctpr3, 1b\n"
     "2:\n" LDW " %1, 0x0, %0\n"
     "   cmpesb  %0, %2, %%pred0\n"
     "   wait    all_c = 1, fl_c = 1\n"
     "   ct      %%ctpr2 ? %%pred0\n"
     "   ct      %%ctpr3\n"
     "3: "
     : "=&r" (tmp)
     : "r" (lock), "r" (locked)
     : "memory", 
       "%ctpr1",
       "%ctpr2",
       "%ctpr3",
       "%pred0");
  return 0;
}
