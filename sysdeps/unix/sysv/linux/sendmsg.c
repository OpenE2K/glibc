/* Compatibility implementation of sendmsg.
   Copyright (C) 2016-2019 Free Software Foundation, Inc.
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
#include <shlib-compat.h>

ssize_t
__libc_sendmsg (int fd, const struct msghdr *msg, int flags)
{
# if defined __ptr128__ && ! defined __ptr128_new_abi__
  struct
  {
    long int a;
    void *b;
    long int c;
  }
  args = {(long int) fd, (void *) msg, (long int) flags};
  
  return SYSCALL_CANCEL (socketcall, SOCKOP_sendmsg, &args);

# elif defined __ASSUME_SENDMSG_SYSCALL
  return SYSCALL_CANCEL (sendmsg, fd, msg, flags);
# else
  return SOCKETCALL_CANCEL (sendmsg, fd, msg, flags);
# endif
}
weak_alias (__libc_sendmsg, sendmsg)
weak_alias (__libc_sendmsg, __sendmsg)
