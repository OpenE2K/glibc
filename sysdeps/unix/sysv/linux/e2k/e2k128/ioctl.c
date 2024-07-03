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

#include <stdarg.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sysdep.h>

typedef struct
{
  unsigned int dummy[2];
  unsigned int curptr;
  unsigned int size;
} array_ptr;


int
__ioctl (int fd, unsigned long int request, ...)
{
  int i;
  va_list ap;
  int result;
  void *args[4] = {NULL, NULL, NULL, NULL};

  va_start (ap, request);

  for (i = 0; i < 4; ++i)
    {
      array_ptr aptr = *((array_ptr *) &ap);
      /* There should be enough space for the next argument. FIXME(?): this
	 test seems to be OK for the new PM ABI, but should it be adapted to
	 the old one? Presumably NOT as with old PM ABI in use it seems to be
	 impossible in principle to fetch the next argument unless it's cast
	 to (void *). */
      if (aptr.curptr + 0xf >= aptr.size)
	break;

      args[i] = va_arg (ap, void *);
    }

  va_end (ap);

  result = INLINE_SYSCALL (ioctl, 6, fd, request, args[0], args[1], args[2],
			   args[3]);
  return result;
}
libc_hidden_def (__ioctl)
weak_alias (__ioctl, ioctl)
