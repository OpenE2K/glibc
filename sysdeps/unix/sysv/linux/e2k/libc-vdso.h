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

/* This file is included from `sysdeps/unix/sysv/linux/clock_gettime.c'
   if we declare that we have the corresponding fast system call (vsyscall).
   Since on E2K we don't actually have anything like real VDSO on other
   platforms, there is nothing to do here.  */
#ifndef _LIBC_E2K_VDSO_H
#define _LIBC_E2K_VDSO_H

#endif /* _LIBC_E2K_VDSO_H */
