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

#include <setjmp.h>
#include <stdio.h>

extern void ____longjmp_chk (__jmp_buf __env, int __val)
  __attribute__ ((__noreturn__));

void
____longjmp_chk (__jmp_buf env, int val)
{
  /* On E2K `longjmp ()' is implemented by means of a system call. Therefore,
     the Kernel should be capable of performing all necessary checks and return
     in case of an invalid ENV. FIXME: it's likely to kill the process under
     some circumstances instead, which is to be revised.  */
    __longjmp (env, val);

  __fortify_fail ("longjmp causes uninitialized stack frame");
}
