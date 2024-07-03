/* Compatibility implementation of sendmsg.
   Copyright (C) 2016-2022 Free Software Foundation, Inc.
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
#include <shlib-compat.h>

ssize_t
__libc_sendmsg (int fd, const struct msghdr *msg, int flags)
{
# ifdef __ASSUME_SENDMSG_SYSCALL
  return SYSCALL_CANCEL (sendmsg, fd, msg, flags);
# else
  return SOCKETCALL_CANCEL (sendmsg, fd, msg, flags);
# endif
}

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__libc_sendmsg, sendmsg)

#else /* defined __e2k__ && defined SHARED  */

# include <shlib-compat.h>

weak_alias (__libc_sendmsg, __libc_sendmsg_weak)
versioned_symbol (libc, __libc_sendmsg_weak, sendmsg, GLIBC_2_0);
compat_symbol (libpthread, __libc_sendmsg_weak, sendmsg, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */

weak_alias (__libc_sendmsg, __sendmsg)
#if __TIMESIZE != 64
weak_alias (__sendmsg, __sendmsg64)
#endif
