/* Lock I/O stream.  Singlethreaded version.
   Copyright (C) 1996-2022 Free Software Foundation, Inc.
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

#include <stdio.h>
#include <stdio-lock.h>

void
__flockfile (FILE *stream)
{
  stream->_flags2 |= _IO_FLAGS2_NEED_LOCK;
  _IO_lock_lock (*stream->_lock);
}

#if ! (defined __e2k__ && defined SHARED)

weak_alias (__flockfile, flockfile);
weak_alias (__flockfile, _IO_flockfile)

#else /* defined __e2k__ && defined SHARED  */

weak_alias (__flockfile, __flockfile_weak)

# include <shlib-compat.h>

/* These symbols were also present in libc-2.29.so. Note that they don't
   take care of preserving the original non-WEAKness of _IO_-symbol it
   used to have (see `weak_alias ()'es above). See also below.  */
versioned_symbol (libc, __flockfile_weak, flockfile, GLIBC_2_0);
versioned_symbol (libc, __flockfile_weak, _IO_flockfile, GLIBC_2_0);

/* For the sake of runtime compatibility with libpthread-2.29.so. Note that
   _IO_flockfile@@GLIBC_2.0 used to be created as strong_alias () in it
   which was "accidentally" changed when getting rid of the implementation in
   sysdeps/pthread.  */
compat_symbol (libpthread, __flockfile_weak, flockfile, GLIBC_2_0);

/* This extra strong_alias () is required because `__f{,un}lockfile ()'s turn
   out to be HIDDEN unlike `__ftrylockfile ()' (???!!!) and the created dynamic
   symbol should not inherit this inappropriate visibility which is sure to
   eventually make it LOCAL.  */
strong_alias (__flockfile, __flockfile_strong)
compat_symbol (libpthread, __flockfile_strong, _IO_flockfile, GLIBC_2_0);

#endif /* defined __e2k__ && defined SHARED  */
