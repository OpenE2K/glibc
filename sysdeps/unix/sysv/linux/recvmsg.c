/* Linux recvmsg syscall wrapper.
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
__libc_recvmsg (int fd, struct msghdr *msg, int flags)
{
# if defined __ptr128__ && ! defined __ptr128_new_abi__

  struct pm_iovec
  {
    long iov_base;
    size_t iov_len;
  };

  struct pm_msghdr
  {
    long msg_name;
    socklen_t msg_namelen;

    long msg_iov;
    int msg_iovlen;

    long msg_control;
    socklen_t msg_controllen;

    int msg_flags;
  };

  ssize_t res;
  int i;
  int iovlen;
  struct pm_msghdr pm_msg;
  struct pm_iovec *pm_msg_iov;


  iovlen = msg->msg_iovlen;
  pm_msg_iov = (struct pm_iovec *) malloc (iovlen * sizeof (struct pm_iovec));

  for (i = 0; i < iovlen; i++)
    {
      pm_msg_iov[i].iov_base = (long) msg->msg_iov[i].iov_base;
      pm_msg_iov[i].iov_len = msg->msg_iov[i].iov_len;
    }

  pm_msg.msg_name = (long) msg->msg_name;
  pm_msg.msg_namelen = msg->msg_namelen;
  pm_msg.msg_iov = (long) pm_msg_iov;
  pm_msg.msg_iovlen = msg->msg_iovlen;
  pm_msg.msg_control = (long) msg->msg_control;
  pm_msg.msg_controllen = msg->msg_controllen;
  pm_msg.msg_flags = msg->msg_flags;

  struct
  {
    long int a;
    void *b;
    long int c;
  }
  args = {(long int) fd, (void *) &pm_msg, (long int) flags};
  
  res = SYSCALL_CANCEL (socketcall, SOCKOP_recvmsg, &args);

  free (pm_msg_iov);
  return res;

# elif defined __ASSUME_RECVMSG_SYSCALL
  return SYSCALL_CANCEL (recvmsg, fd, msg, flags);
# else
  return SOCKETCALL_CANCEL (recvmsg, fd, msg, flags);
# endif
}
weak_alias (__libc_recvmsg, recvmsg)
weak_alias (__libc_recvmsg, __recvmsg)
