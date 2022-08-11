/* Auxiliary vector processing for Linux/E2k.
   Copyright (C) 2007 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* Scan the Aux Vector for AT_FAST_SYSCALL entry.  */

unsigned long __libc_e2k_fast_syscalls;
unsigned long __libc_e2k_system_info;

#define DL_PLATFORM_AUXV				\
  case 32: /* AT_FAST_SYSCALLS:  */                     \
	__libc_e2k_fast_syscalls = av->a_un.a_val;	\
	break;                                          \
  case 34: /* AT_SYSTEM_INFO:  */                       \
        __libc_e2k_system_info = av->a_un.a_val;        \
        break;
