/* Copyright (C) 2015-2019 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <signal.h>
#include <sys/socket.h>

#include <socketcall.h>
#include <kernel-features.h>
#include <sys/syscall.h>

int
setsockopt (int fd, int level, int optname, const void *optval, socklen_t len)
{
#if defined __ptr128__
  struct
  {
    long int a;
    long int b;
    long int c;
    void *d;
    long int e;
  }
  args = {(long int) fd, (long int) level, (long int) optname,
	  (void *) optval, (long int) len};

  return INLINE_SYSCALL (socketcall, 2, SOCKOP_setsockopt, &args);
#elif defined __ASSUME_SETSOCKOPT_SYSCALL
  return INLINE_SYSCALL (setsockopt, 5, fd, level, optname, optval, len);
#else
  return SOCKETCALL (setsockopt, fd, level, optname, optval, len);
#endif
}
weak_alias (setsockopt, __setsockopt)
