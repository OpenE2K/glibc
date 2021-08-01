/* This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sysdep.h>

        .ignore ld_st_style
	.text

ENTRY (syscall)

        setwd   wsz = 0x9
        setbn   rsz = 0x3, rbs = 0x5, rcur = 0x0
        getsp   0x0, %r7

        __SYSCALL_ARG_MEM (%r7, 0x0, %b[0])
        __SYSCALL_ARG_MEM (%r7, 0x8, %b[1])
        __SYSCALL_ARG_MEM (%r7, 0x10, %b[2])
        __SYSCALL_ARG_MEM (%r7, 0x18, %b[3])
        __SYSCALL_ARG_MEM (%r7, 0x20, %b[4])
        __SYSCALL_ARG_MEM (%r7, 0x28, %b[5])
        __SYSCALL_ARG_MEM (%r7, 0x30, %b[6])

        sdisp   %ctpr1, __SYSCALL_TRAPNUM
        call    %ctpr1, wbs = 0x5

        __SYSCALL_OUTPUT

	ret

PSEUDO_END (syscall)
