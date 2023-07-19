#ifndef _LINUX_E2K128_SYSDEP_H
#define _LINUX_E2K128_SYSDEP_H 1

#include <sysdeps/unix/e2k/sysdep.h>

#define __LDD	ldapd
#define __LDW	ldapw
#define __STD	stapd
#define __STW	stapw
#define __GETPL getpl
#define __CMPE  cmpesb
/* FIXME: a pointless clone2 () in PM uses the same (obviously unworkable)
   implementation requiring __ADDP as ordinary modes. TODO: consider
   eliminating in entirely or using a stub returning sth like -ENOSYS.  */
#define __ADDP  addd

# define __SYSCALL_TRAPNUM 8
# define __VSYSCALL_TRAPNUM 7



#ifdef __ASSEMBLER__

# define __SET_SYSCALL_NAME(syscall_name)			\
  addd    0x0, SYS_ify(syscall_name), %db[0];			\
  addd 0x0, 0x0, %db[1]

# define __SYSCALL_ARG_REG_1(src, dst)	movtq %qr2, %qb[2]
# define __SYSCALL_ARG_REG_2(src, dst)	movtq %qr4, %qb[4]
# define __SYSCALL_ARG_REG_3(src, dst)	movtq %qr6, %qb[6]
# define __SYSCALL_ARG_REG_4(src, dst)  movtq %qr8, %qb[8]
# define __SYSCALL_ARG_REG_5(src, dst)  movtq %qr10, %qb[10]

#define __SYSCALL_ARG_REG(src, dst)

#else /* ! defined __ASSEMBLER__  */

# define __p1(x) #x
# define __p2(x) __p1(x)

#include <errno.h>

# define INTERNAL_BOGUS_SYSCALL(name, nr, args...)		\
  __INTERNAL_SYSCALL_NCS (BOGUS_SYSCALL, __SYSCALL_TRAPNUM, __NR_##name, nr, args)

# define ___INTERNAL_SYSCALL_SYSCALL_RES(var)	\
  register long var

# define ___INTERNAL_VSYSCALL_SYSCALL_RES(var)	\
  ___INTERNAL_SYSCALL_SYSCALL_RES (var)

# define ___INTERNAL_BOGUS_SYSCALL_SYSCALL_RES(var)	\
  register void *var

/* FIXME: when invoked via INTERNAL_SYSCALL_NCS SORT is empty. */
# define ___INTERNAL__SYSCALL_RES(var)	\
  register long var

# define __INTERNAL_SYSCALL_RES(sort, var)	\
  ___INTERNAL_##sort##_SYSCALL_RES (var)


# define ___INTERNAL_BOGUS_SYSCALL_SYSCALL_ERR(e)	\
  long e

# define ___INTERNAL_SYSCALL_SYSCALL_ERR(e)

# define __INTERNAL_SYSCALL_ERR(sort, e)		\
  ___INTERNAL_##sort##_SYSCALL_ERR (e)


# define ___INTERNAL_SYSCALL_SYSCALL_OUT_CONSTRAINTS(r)	\
  [res] "=r" (r)

# define ___INTERNAL_BOGUS_SYSCALL_SYSCALL_OUT_CONSTRAINTS(r)	\
  [res] "=r" (r), [err] "=r" (__err)


# define __INTERNAL_SYSCALL_OUT_CONSTRAINTS(sort, var)	\
  ___INTERNAL_##sort##_SYSCALL_OUT_CONSTRAINTS (var)


# define __INTERNAL_SYSCALL_IN_CONSTRAINTS_0(name, nonexistent)  \
  [num] "r" ((void *) name)

# define __INTERNAL_SYSCALL_IN_CONSTRAINTS_1(name, a1)           \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_0 (name,),                  \
    [arg1] "r" ((void *) (a1))

# define __INTERNAL_SYSCALL_IN_CONSTRAINTS_2(name, a1, a2)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_1 (name, a1),               \
    [arg2] "r" ((void *) (a2))

# define __INTERNAL_SYSCALL_IN_CONSTRAINTS_3(name, a1, a2, a3)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_2 (name, a1, a2),               \
    [arg3] "r" ((void *) (a3))


# define __INTERNAL_SYSCALL_IN_CONSTRAINTS_4(name, a1, a2, a3, a4)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_3 (name, a1, a2, a3),               \
    [arg4] "r" ((void *) (a4))

# define __INTERNAL_SYSCALL_IN_CONSTRAINTS_5(name, a1, a2, a3, a4, a5)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_4 (name, a1, a2, a3, a4),		\
    [arg5] "r" ((void *) (a5))

# define __INTERNAL_SYSCALL_IN_CONSTRAINTS_6(name, a1, a2, a3, a4, a5, a6) \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_5 (name, a1, a2, a3, a4, a5),	\
    [arg6] "r" ((void *) (a6))


# define __INTERNAL_SYSCALL_LOAD_ARGS_0		\
  "movtq %[num], %%b[0]\n\t"

# define __INTERNAL_SYSCALL_LOAD_ARGS_1		 \
  __INTERNAL_SYSCALL_LOAD_ARGS_0		 \
  "movtq %[arg1], %%b[2]\n\t"

# define __INTERNAL_SYSCALL_LOAD_ARGS_2		\
  __INTERNAL_SYSCALL_LOAD_ARGS_1                \
  "movtq %[arg2], %%b[4]\n\t"

# define __INTERNAL_SYSCALL_LOAD_ARGS_3          \
  __INTERNAL_SYSCALL_LOAD_ARGS_2                \
  "movtq %[arg3], %%b[6]\n\t"

# define __INTERNAL_SYSCALL_LOAD_ARGS_4          \
  __INTERNAL_SYSCALL_LOAD_ARGS_3                \
  "movtq %[arg4], %%b[8]\n\t"

# define __INTERNAL_SYSCALL_LOAD_ARGS_5          \
  __INTERNAL_SYSCALL_LOAD_ARGS_4                \
  "movtq %[arg5], %%b[10]\n\t"

# define __INTERNAL_SYSCALL_LOAD_ARGS_6          \
  __INTERNAL_SYSCALL_LOAD_ARGS_5                \
  "movtq %[arg6], %%b[12]\n\t"

# define __INTERNAL_SYSCALL_SYSCALL_LOAD_RES	\
  "addd 0x0, %%db[0], %[res]\n\t"

# define __INTERNAL_BOGUS_SYSCALL_SYSCALL_LOAD_RES	\
  "addd 0x0, %%db[0], %[err]\n\t"			\
  "movtq %%qb[2], %[res]\n\t"

# define __INTERNAL_SYSCALL_LOAD_RES(sort)	\
  __INTERNAL_##sort##_SYSCALL_LOAD_RES


# define __INTERNAL_SYSCALL_SYSCALL_RETURN_RES(r)	\
  (r)

# define __INTERNAL_BOGUS_SYSCALL_SYSCALL_RETURN_RES(r)	\
  (__err == 0 ? (r) : (void *) (-1L * (long) r))

# define __INTERNAL_SYSCALL_RETURN_RES(sort, r)	\
  __INTERNAL_##sort##_SYSCALL_RETURN_RES (r)

#endif /* ! defined __ASSEMBLER__  */

#define INLINE_BOGUS_SYSCALL(name, nr, args...)       \
  __INLINE_SYSCALL (BOGUS_SYSCALL, name, nr, args)

#include <sysdeps/unix/sysv/linux/e2k/sysdep.h>

/* FIXME: this strange implementation is currently required only for bogus
   PM syscalls returning an AP so as to ensure that the originally negative
   return value doesn't become positive as a result of a cast from `void *'
   to `long'. However, it may mistakenly convert a large enough positive return
   value into a negative one. For normal PM syscalls this risk is absolutely
   unjustified, therefore think of splitting this macro into bogus and normal
   variants.  */
#undef INTERNAL_SYSCALL_ERROR_P
#define INTERNAL_SYSCALL_ERROR_P(val, err)			\
  ((unsigned long ) ((((long) val) << 16) >> 16) > -4096UL)


/* Fast syscalls aren't currently supported in Protected Mode at all, are
   they?  */
#undef HAVE_GETTIMEOFDAY_VSYSCALL
#undef HAVE_CLOCK_GETTIME_VSYSCALL
#undef HAVE_GETCPU_VSYSCALL
#undef HAVE_SIGGETMASK_VSYSCALL

#ifdef __NR_fast_sys_getcontext
# define HAVE_GETCONTEXT_VSYSCALL	1
#endif /* __NR_fast_sys_getcontext  */

#endif /* _LINUX_E2K128_SYSDEP_H  */
