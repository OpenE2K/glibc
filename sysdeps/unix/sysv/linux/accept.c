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

int
__libc_accept (int fd, __SOCKADDR_ARG addr, socklen_t *len)
{
#if defined __ASSUME_ACCEPT_SYSCALL
  return SYSCALL_CANCEL (accept, fd, addr.__sockaddr__, len);
#elif defined __ASSUME_ACCEPT4_SYSCALL
  return SYSCALL_CANCEL (accept4, fd, addr.__sockaddr__, len, 0);
#else
  return SOCKETCALL_CANCEL (accept, fd, addr.__sockaddr__, len);
#endif
}
weak_alias (__libc_accept, accept)

#if ! (defined __e2k__ && defined SHARED)

libc_hidden_def (accept)

#else /* defined __e2k__ && defined SHARED  */
/* Note that it's __GI_accept that is made WEAK by weak_alias (...) a few lines
   above, not accept and thus not accept@@GLIBC_2.2 which is actually GLOBAL in
   up-to-date libc.so.  */
# include <shlib-compat.h>
/* This strong alias is required in order to get rid of HIDDEN inherent to
   __libc_accept.  */
strong_alias (__libc_accept, __accept_strong)
versioned_symbol (libc, __accept_strong, accept, GLIBC_2_0);

/* However, in libpthread-2.29.so accept@@GLIBC_2.0 was WEAK.  */
weak_alias (__libc_accept, __accept_2_0_weak)
compat_symbol (libpthread, __accept_2_0_weak, accept, GLIBC_2_0);
#endif /* defined __e2k__ && defined SHARED  */
