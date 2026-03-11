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
__libc_send (int fd, const void *buf, size_t len, int flags)
{
#if defined __ASSUME_SEND_SYSCALL
  return SYSCALL_CANCEL (send, fd, buf, len, flags);
#elif defined __ASSUME_SENDTO_SYSCALL
  return SYSCALL_CANCEL (sendto, fd, buf, len, flags, NULL, 0);
#else
  return SOCKETCALL_CANCEL (send, fd, buf, len, flags);
#endif
}

#if ! (defined __e2k__ && defined SHARED)
/* Prevent send from being made a WEAK alias of __libc_send in SHARED case for
   e2k without an explicitly specified default version.  */
weak_alias (__libc_send, send)
#endif /* defined __e2k__ && defined SHARED  */

/* No harm for e2k libc.so as it's the HIDDEN __GI___send that is made a WEAK
   alias of __libc_send in fact.  */
weak_alias (__libc_send, __send)

#if ! (defined __e2k__ && defined SHARED)

/* Prevent __send from being output as a GLOBAL alias of __GI___send without
   an explicitly specified version.  */
libc_hidden_def (__send)

#else /* defined __e2k__ && defined SHARED  */

# include <shlib-compat.h>

/* Whereas default {__,}send@@GLIBC_2.2 are GLOBAL and WEAK respectively
   "as usual" ...  */
versioned_symbol (libc, __libc_send, __send, GLIBC_2_0);
weak_alias (__libc_send, __libc_send_weak)
versioned_symbol (libc, __libc_send_weak, send, GLIBC_2_0);

/* ... both {__,}send@GLIBC_2.0 compat symbols are WEAK by analogy with
   libpthread-2.29.so.  */
compat_symbol (libpthread, __libc_send_weak, __send, GLIBC_2_0);
compat_symbol (libpthread, __libc_send_weak, send, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
