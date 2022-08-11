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

#include <sys/sem.h>
#include <ipc_priv.h>
#include <sysdep.h>
#include <errno.h>

/* Return identifier for array of NSEMS semaphores associated with
   KEY.  */

int
semget (key_t key, int nsems, int semflg)
{
#if defined __ptr128__ && ! defined __ptr128_new_abi__
  struct
  {
    long int a;
    long int b;
    long int c;
    void *d;
  }
  args = {(long int) key, (long int) nsems, (long int) semflg, (void *) NULL};

  return INLINE_SYSCALL_CALL (ipc, IPCOP_semget, &args);

#elif defined __ASSUME_DIRECT_SYSVIPC_SYSCALLS
  return INLINE_SYSCALL_CALL (semget, key, nsems, semflg);
#else
  return INLINE_SYSCALL_CALL (ipc, IPCOP_semget, key, nsems, semflg, NULL);
#endif
}
