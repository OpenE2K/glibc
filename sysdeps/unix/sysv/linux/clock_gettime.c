/* clock_gettime -- Get current time from a POSIX clockid_t.  Linux version.
   Copyright (C) 2003-2022 Free Software Foundation, Inc.
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
#include <kernel-features.h>
#include <errno.h>
#include <time.h>
#include "kernel-posix-cpu-timers.h"
#if defined __e2k__ && defined HAVE_CLOCK_GETTIME_VSYSCALL
/* FIXME: HAVE_VSYSCALL ensures that INLINE_VSYSCALL will attempt to perform
   the actual fast syscall instead of being expanded just as INLINE_SYSCALL.
   Consider eliminating it from glibc entirely by placing all invocations of
   INLINE_VSYSCALL () under respective `ifdef HAVE_..._VSYSCALL'
   conditionals .  */
# define HAVE_VSYSCALL
#endif /* defined __e2k__ && defined HAVE_CLOCK_GETTIME_VSYSCALL  */
#include <sysdep-vdso.h>
#include <shlib-compat.h>

/* Get current value of CLOCK and store it in TP.  */
int
__clock_gettime64 (clockid_t clock_id, struct __timespec64 *tp)
{
  int r;

#if ! defined __e2k__ || __WORDSIZE == 64
#ifndef __NR_clock_gettime64
# define __NR_clock_gettime64 __NR_clock_gettime
#endif /* ! defined __e2k__ || __WORDSIZE == 64  */

#if defined __e2k__ && __WORDSIZE == 64 && defined HAVE_CLOCK_GETTIME_VSYSCALL
# define __NR_fast_sys_clock_gettime64 __NR_fast_sys_clock_gettime
  /* FIXME: ensure that this fast syscall is actually used below.  */
# define HAVE_CLOCK_GETTIME64_VSYSCALL 1
#endif /* defined __e2k__ && __WORDSIZE == 64 && defined HAVE_CLOCK_GETTIME_VSYSCALL  */

#endif

#ifdef HAVE_CLOCK_GETTIME64_VSYSCALL
# if ! defined __e2k__

  int (*vdso_time64) (clockid_t clock_id, struct __timespec64 *tp)
    = GLRO(dl_vdso_clock_gettime64);
  if (vdso_time64 != NULL)
    {
      r = INTERNAL_VSYSCALL_CALL (vdso_time64, 2, clock_id, tp);
      if (r == 0)
	return 0;
      return INLINE_SYSCALL_ERROR_RETURN_VALUE (-r);
    }

# else /* defined __e2k__  */

  {
    int prior_errno = errno;
    r = INLINE_VSYSCALL (clock_gettime64, 2, clock_id, tp);
    if (r == 0 || errno != ENOSYS)
      return r;
    else
      /* The case of `R != 0 && errno == ENOSYS' requires further attempts,
	 so restore the original ERRNO so as not to clobber it for no good
	 reason in case these attempts prove to be successful.  */
      __set_errno (prior_errno);
  }

# endif /* defined __e2k__  */
#endif


#if defined HAVE_CLOCK_GETTIME_VSYSCALL
# if ! defined __e2k__

  int (*vdso_time) (clockid_t clock_id, struct timespec *tp)
    = GLRO(dl_vdso_clock_gettime);
  if (vdso_time != NULL)
    {
      struct timespec tp32;
      r = INTERNAL_VSYSCALL_CALL (vdso_time, 2, clock_id, &tp32);
      if (r == 0 && tp32.tv_sec >= 0)
	{
	  *tp = valid_timespec_to_timespec64 (tp32);
	  return 0;
	}
      else if (r != 0)
	return INLINE_SYSCALL_ERROR_RETURN_VALUE (-r);

      /* Fallback to syscall if the 32-bit time_t vDSO returns overflows.  */
    }

# elif /* defined __e2k__ && */ ! defined HAVE_CLOCK_GETTIME64_VSYSCALL
  
  /* On e2k{64,128} with 64-bit (?) timespec it would be a pointless duplication
     of the above code as __NR_fast_sys_clock_gettime64 may be nothing but __NR_
     fast_sys_clock_gettime for them and is missing outherwise (i.e. for e2k32).
     What about other architectures with 64-bit timespec? I guess that their
     vdso_time{64,} should also be the same and HAVE_CLOCK_GETTIME64_VSYSCALL
     should imply HAVE_CLOCK_GETTIME_VSYSCALL. But the way of recognizing this
     duplication appropriate for e2k in the absence of a separate __NR_fast_sys_
     clock_gettime64 on e2k32 would break the logic for 32-bit architectures
     with dedicated `vdso_time64 ()'. Presumably  the additional test

     ...
     # if defined HAVE_CLOCK_GETTIME64
     && vdso_time != vdso_time64
     # endif 
     ...

     would be the right thing. Alternatively one could attempt to implement a
     more sophisticated compile time conditional to recognize this case.  */
  {
    int prior_errno = errno;
    struct timespec tp32;
    r = INLINE_VSYSCALL (clock_gettime, 2, clock_id, &tp32);
    if (r == 0 && tp32.tv_sec > 0)
      {
	*tp = valid_timespec_to_timespec64 (tp32);
	return 0;
      }
    /* According to the logic in the original code a few lines above, when the
       retrieved time does NOT fit into TP32 R should be 0 and the only sign
       of a failure is `TP32.tv_sec <= 0'.  */
    else if (r != 0 && errno != ENOSYS)
      return r;
    else
      /* One may find himself here in two cases: `R != 0 && errno == ENOSYS'
	 or `TP32.tv_sec < 0'. In both of them further attempts should be
	 made, so restore the original ERRNO so as not to clobber it if
	 these attempts turn out to be successful.  */
      __set_errno (prior_errno);
  }

# endif /* defined __e2k__  */
#endif

#if ! defined __e2k__ || __WORDSIZE == 64
  r = INTERNAL_SYSCALL_CALL (clock_gettime64, clock_id, tp);
  if (r == 0)
    return 0;
  if (r != -ENOSYS)
    return INLINE_SYSCALL_ERROR_RETURN_VALUE (-r);
#endif /* ! defined __e2k__ || __WORDSIZE == 64  */

#ifndef __ASSUME_TIME64_SYSCALLS
  /* Fallback code that uses 32-bit support.  */
  struct timespec tp32;
  r = INTERNAL_SYSCALL_CALL (clock_gettime, clock_id, &tp32);
  if (r == 0)
    {
      *tp = valid_timespec_to_timespec64 (tp32);
      return 0;
    }
#endif

  return INLINE_SYSCALL_ERROR_RETURN_VALUE (-r);
}

#if __TIMESIZE != 64
libc_hidden_def (__clock_gettime64)

int
__clock_gettime (clockid_t clock_id, struct timespec *tp)
{
  int ret;
  struct __timespec64 tp64;

  ret = __clock_gettime64 (clock_id, &tp64);

  if (ret == 0)
    {
      if (! in_time_t_range (tp64.tv_sec))
        {
          __set_errno (EOVERFLOW);
          return -1;
        }

      *tp = valid_timespec64_to_timespec (tp64);
    }

  return ret;
}
#endif
libc_hidden_def (__clock_gettime)

versioned_symbol (libc, __clock_gettime, clock_gettime, GLIBC_2_17);
/* clock_gettime moved to libc in version 2.17;
   old binaries may expect the symbol version it had in librt.  */
#if SHLIB_COMPAT (libc, GLIBC_2_2, GLIBC_2_17)
strong_alias (__clock_gettime, __clock_gettime_2);
compat_symbol (libc, __clock_gettime_2, clock_gettime, GLIBC_2_2);
#endif
