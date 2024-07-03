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

#include <errno.h>
#include <stdarg.h>
#include <lowlevellock-futex.h>


int
futex (void *uaddr, int futex_op, int val, ...)
{
  int res = -ENOSYS;

  if (futex_op == FUTEX_WAKE)
    res = lll_futex_syscall (4, uaddr, futex_op, val, 0);
  else if (futex_op == FUTEX_WAIT)
    {
      va_list ap;
      void *timeout;

      va_start (ap, val);
      timeout = va_arg (ap, void *);
      res = lll_futex_syscall (4, uaddr, futex_op, val, timeout);
      va_end (ap);
    }

  if (res != 0)
    {
      errno = -res;
      res = -1;
    }

  return res;
}
