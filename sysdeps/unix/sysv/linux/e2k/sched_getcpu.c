/* Copyright (C) 2007 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>
#include <sched.h>
#include <sysdep.h>

#ifdef HAVE_GETCPU_VSYSCALL
# define HAVE_VSYSCALL
#endif /* HAVE_GETCPU_VSYSCALL  */
#include <sysdep-vdso.h>

int
sched_getcpu (void)
{
  unsigned int cpu;
  int r = INLINE_VSYSCALL (getcpu, 3, &cpu, NULL, NULL);

  return r == -1 ? r : cpu;
}
