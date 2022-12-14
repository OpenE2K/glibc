/* Copyright (C) 1997-2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Jaeger <aj@suse.de>, 1997.

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

#include "test-float.h"
#include "test-math-inline.h"
#include "test-math-scalar.h"

<<<<<<< HEAD
#define TEST_MSG "testing float (inline functions)\n"

#ifdef __LCC__
/* See the corresponding detailed comment in `libm-test.inc' for why this macro
   is required. In brief, it fixes the compilation problem of `test-float.c'.  */
# define __BUG_65596__
#endif /* __LCC__  */

=======
>>>>>>> 40b7fbf27310d4f4a899aa2c54740dce5369ec70
#include "libm-test.c"
