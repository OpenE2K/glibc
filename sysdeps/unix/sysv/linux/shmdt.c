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
#include <sysdep.h>
#include <errno.h>

/* Detach shared memory segment starting at address specified by SHMADDR
   from the caller's data segment.  */

int
shmdt (const void *shmaddr)
{
#if defined __ptr128__
  struct
  {
    long int a;
    long int b;
    long int c;
    void *d;
  }
  args = {(long int) 0, (long int) 0, (long int) 0, (void *) shmaddr};

  return INLINE_SYSCALL_CALL (ipc, IPCOP_shmdt, &args);

#elif defined __ASSUME_DIRECT_SYSVIPC_SYSCALLS
  return INLINE_SYSCALL_CALL (shmdt, shmaddr);
#else
  return INLINE_SYSCALL_CALL (ipc, IPCOP_shmdt, 0, 0, 0, shmaddr);
#endif
}
