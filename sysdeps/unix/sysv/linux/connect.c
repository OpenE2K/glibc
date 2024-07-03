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
__libc_connect (int fd, __CONST_SOCKADDR_ARG addr, socklen_t len)
{
#if defined __ptr128__
  struct
  {
    long int a;
    void *b;
    long int c;
  }
  args = {(long int) fd, (void *) addr.__sockaddr__, (long int) len};

  return SYSCALL_CANCEL (socketcall, SOCKOP_connect, &args);

#elif defined __ASSUME_CONNECT_SYSCALL
  return SYSCALL_CANCEL (connect, fd, addr.__sockaddr__, len);
#else
  return SOCKETCALL_CANCEL (connect, fd, addr.__sockaddr__, len);
#endif
}
#if ! (defined __e2k__ && defined SHARED)
/* Disable this in SHARED case for e2k as it actually makes connect
   a WEAK alias of __libc_connect, whereas we need connect@@GLIBC_2.2.  */
weak_alias (__libc_connect, connect)
#endif /* ! (defined __e2k__ && defined SHARED)  */

/* No point to disable in e2k SHARED case as __GI___connect is actually made a
   WEAK alias of __libc_connect.  */
weak_alias (__libc_connect, __connect)

#if ! (defined __e2k__ && defined SHARED)

/* Prevent __connect from being output as an alias of __GI___connect without an
   explicitly specified default version.  */
libc_hidden_weak (__connect)

#else /* defined __e2k__ && defined SHARED  */

# include <shlib-compat.h>

/* All of `{__,}connect@{@GLIBC_2.2,GLIBC_2.0}'s should be WEAK.  */
weak_alias (__libc_connect, __libc_connect_weak)

versioned_symbol (libc, __libc_connect_weak, connect, GLIBC_2_0);
versioned_symbol (libc, __libc_connect_weak, __connect, GLIBC_2_0);

compat_symbol (libpthread, __libc_connect_weak, connect, GLIBC_2_0);
compat_symbol (libpthread, __libc_connect_weak, __connect, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
