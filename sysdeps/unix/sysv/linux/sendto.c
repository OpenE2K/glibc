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

#include <sys/socket.h>
#include <sysdep-cancel.h>
#include <socketcall.h>

ssize_t
__libc_sendto (int fd, const void *buf, size_t len, int flags,
	       __CONST_SOCKADDR_ARG addr, socklen_t addrlen)
{
#if defined __ptr128__
  struct
  {
    long int a;
    void *b;
    long int c;
    long int d;
    void *e;
    long int f;
  }
  args = {(long int) fd, (void *) buf, (long int) len, (long int) flags,
	  (void *) addr.__sockaddr__, (long int) addrlen};
  
  return SYSCALL_CANCEL (socketcall, SOCKOP_sendto, &args);

#elif defined __ASSUME_SENDTO_SYSCALL
  return SYSCALL_CANCEL (sendto, fd, buf, len, flags, addr.__sockaddr__,
                         addrlen);
#else
  return SOCKETCALL_CANCEL (sendto, fd, buf, len, flags, addr.__sockaddr__,
			    addrlen);
#endif
}
weak_alias (__libc_sendto, sendto)
weak_alias (__libc_sendto, __sendto)
