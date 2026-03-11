/* Set flags signalling availability of kernel features based on given
   kernel version number.
   Copyright (C) 2010-2014 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _KERNEL_FEATURES_H
#define _KERNEL_FEATURES_H 1

# define __ASSUME_SOCKET_SYSCALL             1
# define __ASSUME_SOCKETPAIR_SYSCALL         1
# define __ASSUME_BIND_SYSCALL               1
# define __ASSUME_LISTEN_SYSCALL             1
# define __ASSUME_GETSOCKNAME_SYSCALL        1
# define __ASSUME_GETPEERNAME_SYSCALL        1
# define __ASSUME_SHUTDOWN_SYSCALL           1

#include_next <kernel-features.h>

#if defined __ptr32__
# define __ASSUME_SYSVIPC_BROKEN_MODE_T
#endif /* defined __ptr32__  */

#endif /* _KERNEL_FEATURES_H */
