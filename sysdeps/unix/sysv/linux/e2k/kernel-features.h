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

#define __ASSUME_SOCKETCALL		1

#include_next <kernel-features.h>

/* Due to some obscure reasons `__NR_{recv,send}msg' syscalls are missing from
   E2K Linux Kernel, whereas `sysdeps/unix/sysv/linux/kernel-features.h' defines
   the following macros by default nowadays.  */
#if ! defined __ptr128__
# undef __ASSUME_RECVMSG_SYSCALL
# undef __ASSUME_SENDMSG_SYSCALL
#endif /* ! defined __ptr128__  */

/* These ones stayed undefined in the generic kernel-features.h by virtue of
   'defined __e2k__' conditionals. Now that they are getting rid of such
   conditionals in the generic kernel-features.h (see commit
   5e7698c6f152c93a73ee4140ad23f7171aa79ce0) undefine them here explicitly.
   FIXME: to be revisited. Probably we may define some of these macros now that
   we work in linux-2.6.33 environment at E2K.  */

/* At E2k we have `signalfd4' syscall starting from linux-2.6.33 (see
   Bug #66978).  */
#if __LINUX_KERNEL_VERSION < 0x020621
# undef __ASSUME_SIGNALFD4
#endif

#undef __ASSUME_AT_RANDOM




/* These ones used to be defined in the generic kernel-features.h for E2K (i.e.
   under '#ifdef __e2k__' conditional) accompanied by the following comment:
   
   Linux at E2k starts from version 2.4.0.

   Move them here. FIXME: find out whether these macros really make any sense
   nowadays. Maybe they lead to duplication.  */
#define __ASSUME_TRUNCATE64_SYSCALL	1
#define __ASSUME_MMAP2_SYSCALL		1
#define __ASSUME_STAT64_SYSCALL		1
#define __ASSUME_FCNTL64		1

/* E2K only supports ipc syscall.  */
#undef __ASSUME_DIRECT_SYSVIPC_SYSCALLS

#undef __ASSUME_ACCEPT_SYSCALL
#undef __ASSUME_ACCEPT4_SYSCALL
#undef __ASSUME_RECVFROM_SYSCALL
#undef __ASSUME_CONNECT_SYSCALL
#undef __ASSUME_SENDTO_SYSCALL

#if defined __ptr128__
# undef __ASSUME_RENAMEAT2
#endif /* defined __ptr128__  */

#endif /* _KERNEL_FEATURES_H */
