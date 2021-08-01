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
listen (int fd, int backlog)
{
#if defined __ptr128__
  struct
  {
    long int a;
    long int b;
    /* FIXME: this field is just to ensure the proper alignment for now.  */
    void *c;
  }
  args = {(long int) fd, (long int) backlog};

  return INLINE_SYSCALL (socketcall, 2, SOCKOP_listen, &args);

#elif defined __ASSUME_LISTEN_SYSCALL
  return INLINE_SYSCALL (listen, 2, fd, backlog);
#else
  return SOCKETCALL (listen, fd, backlog);
#endif
}
weak_alias (listen, __listen);
