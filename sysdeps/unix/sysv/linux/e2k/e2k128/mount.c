/* Copyright (c) 2009-2024 AO MCST.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
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

#include <sysdep.h>
#include <sys/mount.h>

/* This `extern' and subsequent `libc_hidden_proto's would probably be placed
   into `include/sys/mount.h' if it existed. Presumably it's missing because
   originally glibc doesn't allow for `mount ()' to be implemented in C.  */
extern int __mount (const char *source, const char *target,
                    const char *filesystemtype,
                    unsigned long mountflags, const void *data);

libc_hidden_proto (mount)
libc_hidden_proto (__mount)

int
__mount (const char *source, const char *target, const char *filesystemtype,
         unsigned long mountflags, const void *data)
{
  /* Now that only the new PM ABI is supported, consider getting rid of
     this file in favour of generic one which may probably be expected
     to do the right thing.  */
  return INLINE_SYSCALL (mount, 5, source, target, filesystemtype,
			 mountflags, data);
}

/* FIXME: this magic has stupidly been borrowed from many other files like
   `sysdeps/unix/sysv/linux/read.c'. Note, however, that it makes `__GI_mount'
   weak rather than `mount' (due to asm redirection in the above `libc_hidden_
   proto (mount)' in fact, whereas when compiling an automatically generated
   assembler file it's definitely `mount' that becomes weak.  */
weak_alias (__mount, mount)
libc_hidden_def (__mount)
libc_hidden_def (mount)
