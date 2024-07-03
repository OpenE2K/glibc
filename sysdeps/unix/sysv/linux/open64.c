/* Linux open syscall implementation, LFS.
   Copyright (C) 1991-2022 Free Software Foundation, Inc.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sysdep-cancel.h>
#include <shlib-compat.h>

/* Open FILE with access OFLAG.  If O_CREAT or O_TMPFILE is in OFLAG,
   a third argument is the file protection.  */
int
__libc_open64 (const char *file, int oflag, ...)
{
  int mode = 0;

  if (__OPEN_NEEDS_MODE (oflag))
    {
      va_list arg;
      va_start (arg, oflag);
      mode = va_arg (arg, int);
      va_end (arg);
    }

#if ! defined __e2k__
  return SYSCALL_CANCEL (openat, AT_FDCWD, file, oflag | O_LARGEFILE,
			 mode);
#else
  return SYSCALL_CANCEL (open, file, oflag | O_LARGEFILE, mode);
#endif
}

strong_alias (__libc_open64, __open64)
libc_hidden_weak (__open64)
weak_alias (__libc_open64, open64)

#ifdef __OFF_T_MATCHES_OFF64_T
strong_alias (__libc_open64, __libc_open)
strong_alias (__libc_open64, __open)

# if ! (defined __e2k__ && defined SHARED)
/* Prevent {__,}open from being emitted without an explicitly specified version
   in SHARED case for e2k.  */
libc_hidden_weak (__open)
weak_alias (__libc_open64, open)

# else /* defined __e2k__ && defined SHARED  */

weak_alias (__libc_open64, __libc_open64_weak)

/* In 64-bit mode only __open@GLIBC_2.0  added for the sake of runtime
   compatibility with libpthread-2.29.so turns out to be non-WEAK.  */
versioned_symbol (libc, __libc_open64_weak, __open, GLIBC_2_0);
versioned_symbol (libc, __libc_open64_weak, open, GLIBC_2_0);

compat_symbol (libpthread, __libc_open64, __open, GLIBC_2_0);
compat_symbol (libpthread, __libc_open64_weak, open, GLIBC_2_0);

# endif /* defined __e2k__ && defined SHARED  */

#endif

#if ! defined __e2k__ && OTHER_SHLIB_COMPAT (libpthread, GLIBC_2_1, GLIBC_2_2)
compat_symbol (libc, __libc_open64, open64, GLIBC_2_2);
#endif
