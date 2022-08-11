/* Copyright (C) 1995-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gnu.ai.mit.edu>, August 1995.

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

#include <sys/msg.h>
#include <ipc_priv.h>
#include <sysdep-cancel.h>

ssize_t
__libc_msgrcv (int msqid, void *msgp, size_t msgsz, long int msgtyp,
	       int msgflg)
{
#if defined __ptr128__ && ! defined __ptr128_new_abi__
  struct
  {
    long int a;
    long int b;
    long int c;
    void *d;
    long int e;
  }
  args = {(long int) msqid, (long int) msgsz, (long int) msgflg,
	  (void *) msgp, (long int) msgtyp};
  /* FIXME(?): `1 << 16' prevents the Kernel from interpreting the fourth
     argument as the pointer to an array into which MSGP and MSGTYP are
     packed. That would be absolutely redundant as all arguments are already
     packed into a struct in PM.  */
  return SYSCALL_CANCEL (ipc, IPCOP_msgrcv | (1 << 16), &args);

#elif defined __ASSUME_DIRECT_SYSVIPC_SYSCALLS
  return SYSCALL_CANCEL (msgrcv, msqid, msgp, msgsz, msgtyp, msgflg);
#else
  return SYSCALL_CANCEL (ipc, IPCOP_msgrcv, msqid, msgsz, msgflg,
			 MSGRCV_ARGS (msgp, msgtyp));
#endif
}
weak_alias (__libc_msgrcv, msgrcv)
