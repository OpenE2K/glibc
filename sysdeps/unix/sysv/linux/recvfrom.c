/* Copyright (C) 2015-2022 Free Software Foundation, Inc.
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

#include <sys/socket.h>
#include <sysdep-cancel.h>
#include <socketcall.h>

ssize_t
__libc_recvfrom (int fd, void *buf, size_t len, int flags,
		 __SOCKADDR_ARG addr, socklen_t *addrlen)
{
#if defined __ASSUME_RECVFROM_SYSCALL
  return SYSCALL_CANCEL (recvfrom, fd, buf, len, flags, addr.__sockaddr__,
                         addrlen);
#else
  return SOCKETCALL_CANCEL (recvfrom, fd, buf, len, flags, addr.__sockaddr__,
			    addrlen);
#endif
}

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__libc_recvfrom, recvfrom)

#else /* defined __e2k__ && defined SHARED  */

#include <shlib-compat.h>

weak_alias (__libc_recvfrom, __libc_recvfrom_weak)
versioned_symbol (libc, __libc_recvfrom_weak, recvfrom, GLIBC_2_0);
compat_symbol (libpthread, __libc_recvfrom_weak, recvfrom, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */

weak_alias (__libc_recvfrom, __recvfrom)
