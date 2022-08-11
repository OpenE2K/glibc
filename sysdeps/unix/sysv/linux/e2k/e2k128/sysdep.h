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

#ifdef __ptr128_new_abi__

# define __SYSCALL_TRAPNUM 8
# define __VSYSCALL_TRAPNUM 7

#else /* ! defined __ptr128_new_abi__  */

# define __SYSCALL_TRAPNUM 10

#endif /* ! defined __ptr128_new_abi__  */


#ifdef __ASSEMBLER__

#ifdef __ptr128_new_abi__

# define __SET_SYSCALL_NAME(syscall_name)			\
  addd    0x0, SYS_ify(syscall_name), %db[0];			\
  addd 0x0, 0x0, %db[1]

# define __SYSCALL_ARG_REG_1(src, dst)	movtq %qr2, %qb[2]
# define __SYSCALL_ARG_REG_2(src, dst)	movtq %qr4, %qb[4]
# define __SYSCALL_ARG_REG_3(src, dst)	movtq %qr6, %qb[6]
# define __SYSCALL_ARG_REG_4(src, dst)  movtq %qr8, %qb[8]
# define __SYSCALL_ARG_REG_5(src, dst)  movtq %qr10, %qb[10]

#else /* ! defined __ptr128_new_abi__  */

# define __SYSCALL_ARG_MEM(src1, src2, dst)

#endif /* ! defined __ptr128_new_abi__  */

#define __SYSCALL_ARG_REG(src, dst)

/* FIXME: SYSCALL_NAME macro isn't defined when compiling assembler sources
   created by hand. I believe that this should be fixed.  */
# if defined SYSCALL_NAME && ! defined __ptr128_new_abi__

#  define __sysnum(arg) __NR_##arg
#  define _sysnum(arg) __sysnum(arg)
#  define sysnum _sysnum(SYSCALL_NAME)

/* No arguments.  */

#  if sysnum == __NR_getpid || sysnum == __NR_getegid			\
  || sysnum == __NR_geteuid || sysnum == __NR_getpgrp			\
  || sysnum == __NR_getppid || sysnum == __NR_inotify_init		\
  || sysnum == __NR_munlockall || sysnum == __NR_sched_yield		\
  || sysnum == __NR_getgid || sysnum == __NR_getpid			\
  || sysnum == __NR_getuid || sysnum == __NR_setsid			\
  || sysnum == __NR_sync						\
  /* I can't understand why vhangup is said to accept			\
     a single integer parameter in syscalls.list. According		\
     to the prototype of its wrapper and Kernel sources it takes	\
     no parameters at all.  */						\
  || sysnum == __NR_vhangup


/* One argument.  */

/* I  */

#  elif sysnum == __NR_alarm || sysnum == __NR_epoll_create \
  || sysnum == __NR_epoll_create1 || sysnum == __NR_getpgid \
  || sysnum == __NR_getsid || sysnum == __NR_iopl		  \
  || sysnum == __NR_mlockall || sysnum == __NR_sched_getscheduler \
  || sysnum == __NR_sched_get_priority_max || sysnum == __NR_sched_get_priority_min \
  || sysnum == __NR_setfsgid || sysnum == __NR_setfsuid			\
  || sysnum == __NR_unshare || sysnum == __NR_dup \
  || sysnum == __NR_fchdir || sysnum == __NR_syncfs \
  || sysnum == __NR_umask || sysnum == __NR_inotify_init1 \
  || (defined __NR_pkey_free && sysnum == __NR_pkey_free)

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]

/* L  */
#  elif sysnum == __NR_personality

#   define __SYSCALL_ARG_REG_1(src,dst)	addd 0x0, %dr2, %db[1]

/* P  */

# elif sysnum == __NR_adjtimex || sysnum == __NR_get_kernel_syms \
  || sysnum == __NR_stime || sysnum == __NR_sysinfo \
  || sysnum == __NR_swapoff || sysnum == __NR_uselib \
  || sysnum == __NR_chdir || sysnum == __NR_chroot \
  || sysnum == __NR_rmdir || sysnum == __NR_unlink \
  || sysnum == __NR_pipe  || sysnum == __NR_acct

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]


/* Two arguments.  */

/* II  */

#elif sysnum == __NR_eventfd2 || sysnum == __NR_flock	\
  || sysnum == __NR_inotify_rm_watch || sysnum == __NR_setpgid \
  || sysnum == __NR_timerfd_create || sysnum == __NR_fanotify_init \
  || sysnum == __NR_setns || sysnum == __NR_dup2 \
  || sysnum == __NR_fchmod || sysnum == __NR_kill \
  || sysnum == __NR_setpgid

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) sxt 0x2, %r3, %db[2]


/* IL  */

#elif sysnum == __NR_bdflush

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) addd %dr3, %db[2]

/* IP  */
#  elif sysnum == __NR_fstatfs || sysnum == __NR_clock_adjtime \
  || sysnum == __NR_sched_getparam || sysnum == __NR_sched_rr_get_interval \
  || sysnum == __NR_sched_setparam || sysnum == __NR_timerfd_gettime \
  || sysnum == __NR_getgroups || sysnum == __NR_getitimer \
  || sysnum == __NR_getrusage || sysnum == __NR_fremovexattr

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) movtq %qr4, %qb[2]

/* LL  */
#  elif (defined __NR_pkey_alloc && sysnum == __NR_pkey_alloc)

#   define __SYSCALL_ARG_REG_1(src,dst)	addd 0x0, %dr2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) addd 0x0, %dr3, %db[2]

/* PI  */
/* Why does swapon end up to be marked as accepting the only string argument in
   the generated `sysd-syscalls'?  */
#  elif sysnum == __NR_swapon || sysnum == __NR_mkdir	\
  || sysnum == __NR_pipe2 || sysnum == __NR_chmod \
  || sysnum == __NR_delete_module \
  || (defined __NR_memfd_create && sysnum == __NR_memfd_create)

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq	%qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	sxt 0x2, %r4, %db[4]

/* PL  */
#  elif sysnum == __NR_setdomainname || sysnum == __NR_sethostname \
  || sysnum == __NR_sigaltstack \
  || sysnum == __NR_e2k_longjmp2 || sysnum == __NR_munmap	\
  || sysnum == __NR_mlock || sysnum == __NR_munlock

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	addd 0x0, %dr4, %db[4]


/* PP  */

#  elif sysnum == __NR_capget || sysnum == __NR_capset \
  || sysnum == __NR_pivot_root || sysnum == __NR_removexattr \
  || sysnum == __NR_lremovexattr || sysnum == __NR_link \
  || sysnum == __NR_symlink || sysnum == __NR_statfs \
  || sysnum == __NR_settimeofday || sysnum == __NR_utime

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq	%qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	movtq	%qr4, %qb[4]

/* Three arguments.  */


/* III  */

#  elif sysnum == __NR_dup3 || sysnum == __NR_fchown \
  || sysnum == __NR_setpriority

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	sxt 0x2, %r3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	sxt 0x2, %r4, %db[3]

/* IIP  */
#  elif sysnum == __NR_sched_setscheduler


#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	sxt 0x2, %r3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr4, %qb[4]



/* ILL  */

#  elif sysnum == __NR_readahead

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	addd 0x0, %dr3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst) addd 0x0, %dr4, %db[3]

/* IPI  */

#  elif sysnum == __NR_inotify_add_watch || sysnum == __NR_syslog \
  || sysnum == __NR_mkdirat || sysnum == __NR_unlinkat

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	movtq %qr4, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst) sxt 0x2, %r6, %db[4]

/* IPL  */

#  elif sysnum == __NR_flistxattr

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	movtq %qr4, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst) addd 0x0, %dr6, %db[4]


/* IPP  */

#  elif sysnum == __NR_nfsservctl || sysnum == __NR_mq_getsetattr \
  || sysnum == __NR_setitimer

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	movtq %qr4, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst) movtq %qr6, %qb[4]


/* LLI  */
#  elif sysnum == __NR_ioperm

#   define __SYSCALL_ARG_REG_1(src,dst)	addd 0x0, %dr2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	addd 0x0, %dr3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst) sxt 0x2, %r4, %db[3]

/* PII  */
#  elif sysnum == __NR_lchown || sysnum == __NR_chown

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	sxt 0x2, %r4, %db[4]
#   define __SYSCALL_ARG_REG_3(src,dst)	sxt 0x2, %r5, %db[5]


/* PIP  */
#  elif sysnum == __NR_symlinkat

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	sxt 0x2, %r4, %db[4]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr6, %qb[6]



/* PLI  */
#  elif sysnum == __NR_mprotect || sysnum == __NR_madvise

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	addd 0x0, %dr4, %db[4]
#   define __SYSCALL_ARG_REG_3(src,dst)	sxt 0x2, %r5, %db[5]


/* PLP  */
/* mincore isn't probably supported in the Kernel at all, but anyway get ready
   to pass arguments to it.  */
#  elif sysnum == __NR_mincore

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	addd 0x0, %dr4, %db[4]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr6, %qb[6]

/* Note that our Kernel accepts arguments to `init_module ()' in a rather
   perverted order.  */
#  elif sysnum == __NR_init_module

#   define __SYSCALL_ARG_REG_1(src,dst)	addd 0x0, %dr4, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr6, %db[4]


/* PPL  */

#  elif sysnum == __NR_listxattr || sysnum == __NR_llistxattr \
  || sysnum == __NR_readlink

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	movtq %qr4, %db[4]
#   define __SYSCALL_ARG_REG_3(src,dst)	addd 0x0, %dr6, %db[6]


/* PPP  */

#  elif sysnum == __NR_getresuid32 || sysnum == __NR_getresgid32 \
  || sysnum == __NR_execve

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst)	movtq %qr4, %db[4]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr6, %qb[6]



/* Four arguments.  */

/* IIPL  */
#  elif sysnum == __NR_sendfile

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) sxt 0x2, %r3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr4, %qb[4]
#   define __SYSCALL_ARG_REG_4(src,dst) addd 0x0, %dr6, %db[6]

/* IIPP  */
#  elif sysnum == __NR_prlimit64 || sysnum == __NR_timerfd_settime

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) sxt 0x2, %r3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr4, %qb[4]
#   define __SYSCALL_ARG_REG_4(src,dst) movtq %qr6, %db[6]


/* IPIP  */
/* Note the perverted order in which these idiots pass arguments to `sys_wait4
   ()' despite the fact that there are enough registers to pass them in the
   normal order.  */
#  elif sysnum == __NR_wait4

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) movtq %qr4, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	ldapq %qr0, 0x40, %qb[4]
#   define __SYSCALL_ARG_REG_4(src,dst) sxt 0x2, %r6, %db[6]


/* IPPL  */
#  elif sysnum == __NR_readlinkat

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) movtq %qr4, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	movtq %qr6, %qb[4]
/* Note the use of ldapd here.  */
#   define __SYSCALL_ARG_REG_4(src,dst) ldapd %qr0, 0x40, %db[6]


/* PLLI  */
/* I wonder why these idiots pass some pid as the first argument to `mremap ()'.
   Isn't this a misprint? Note that this syscall may occasionally accept 5
   arguments, but we have got no registers for the optional fifth `new_address'
   one . . .  */
#  elif sysnum == __NR_mremap

#   define __SYSCALL_ARG_REG_1(src,dst)	movtq %qr2, %qb[2]
#   define __SYSCALL_ARG_REG_2(src,dst) addd 0x0, %dr4, %db[4]
#   define __SYSCALL_ARG_REG_3(src,dst)	addd 0x0, %dr5, %db[5]
#   define __SYSCALL_ARG_REG_4(src,dst) sxt 0x2, %r6, %db[6]


/* Five arguments.  */

/* IILIP  */

#  elif sysnum == __NR_fanotify_mark

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) sxt 0x2, %r3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst) addd 0x0, %dr4, %db[3]
#   define __SYSCALL_ARG_REG_4(src,dst) sxt 0x2, %r5, %db[4]
#   define __SYSCALL_ARG_REG_5(src,dst)	movtq %qr6, %qb[6]

/* ILLLL  */
#  elif sysnum == __NR_prctl

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) addd 0x0, %r3, %db[2]
#   define __SYSCALL_ARG_REG_3(src,dst) addd 0x0, %dr4, %db[3]
#   define __SYSCALL_ARG_REG_4(src,dst) addd 0x0, %dr5, %db[4]
#   define __SYSCALL_ARG_REG_5(src,dst)	addd 0x0, %dr6, %db[5]

/* IPIII  */
#  elif sysnum == __NR_fchownat

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) movtq %qr4, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst)	sxt 0x2, %r6, %db[4]
#   define __SYSCALL_ARG_REG_4(src,dst)	sxt 0x2, %r7, %db[5]
/* Note that ldapw zero extends its double format result.  */
#   define __SYSCALL_ARG_REG_5(src,dst)	ldapw %qr0, 0x40, %db[6]



/* IPIPI  */

/* Note that the last `__flags' parameter is passed to the Kernel before
   `__to' so as to fit available registers.  */
#  elif sysnum == __NR_linkat

#   define __SYSCALL_ARG_REG_1(src,dst)	sxt 0x2, %r2, %db[1]
#   define __SYSCALL_ARG_REG_2(src,dst) movtq %qr4, %qb[2]
#   define __SYSCALL_ARG_REG_3(src,dst) sxt 0x2, %dr6, %db[4]
#   define __SYSCALL_ARG_REG_4(src,dst) ldapq %qr0, 0x40, %qb[6]
/* LDGDW is likely to zero-extend its result which is what I really need.  */
#   define __SYSCALL_ARG_REG_5(src,dst)	ldapw %qr0, 0x50, %db[5]

/* Let syscalls unsupported by the Kernel fail at runtime.  */
#  elif sysnum == __NR_create_module || sysnum == __NR_getpmsg		\
  || sysnum == __NR_epoll_ctl						\
  /* mount takes too many parameters, which is probably why it's not	\
     supported in PM. Funnily enough, they do support umount accepting a \
     single argument.  */						\
  || sysnum == __NR_mount						\
  || sysnum == __NR_putpmsg						\
  || sysnum == __NR_query_module					\
  || sysnum == __NR_quotactl						\
  || sysnum == __NR_remap_file_pages					\
  || sysnum == __NR_setxattr						\
  || sysnum == __NR_lsetxattr						\
  || sysnum == __NR_fsetxattr						\
  || sysnum == __NR_getxattr						\
  || sysnum == __NR_lgetxattr						\
  || sysnum == __NR_fgetxattr						\
  || sysnum == __NR_name_to_handle_at					\
  || sysnum == __NR_process_vm_readv					\
  || sysnum == __NR_process_vm_writev

#   define __SYSCALL_ARG_REG_1(src,dst) udivs 0x1, 0x0, %db[1]
#  else
#   error "Assembler generated wrapper is unsupported for this sysnum"
#  endif

# endif /* SYSCALL_NAME  */

#else /* ! defined __ASSEMBLER__  */

# define __p1(x) #x
# define __p2(x) __p1(x)

#include <errno.h>

# ifdef __ptr128_new_abi__

# define INTERNAL_BOGUS_SYSCALL(name, nr, args...)		\
  __INTERNAL_SYSCALL_NCS (BOGUS_SYSCALL, __SYSCALL_TRAPNUM, __NR_##name, nr, args)

#define ___INTERNAL_SYSCALL_SYSCALL_RES(var)	\
  register long var

#define ___INTERNAL_VSYSCALL_SYSCALL_RES(var)	\
  ___INTERNAL_SYSCALL_SYSCALL_RES (var)

#define ___INTERNAL_BOGUS_SYSCALL_SYSCALL_RES(var)	\
  register void *var

/* FIXME: when invoked via INTERNAL_SYSCALL_NCS SORT is empty. */
#define ___INTERNAL__SYSCALL_RES(var)	\
  register long var

#define __INTERNAL_SYSCALL_RES(sort, var)	\
  ___INTERNAL_##sort##_SYSCALL_RES (var)


#define ___INTERNAL_BOGUS_SYSCALL_SYSCALL_ERR(e)	\
  long e

#define ___INTERNAL_SYSCALL_SYSCALL_ERR(e)

#define __INTERNAL_SYSCALL_ERR(sort, e)		\
  ___INTERNAL_##sort##_SYSCALL_ERR (e)


#define ___INTERNAL_SYSCALL_SYSCALL_OUT_CONSTRAINTS(r)	\
  [res] "=r" (r)

#define ___INTERNAL_BOGUS_SYSCALL_SYSCALL_OUT_CONSTRAINTS(r)	\
  [res] "=r" (r), [err] "=r" (__err)


#define __INTERNAL_SYSCALL_OUT_CONSTRAINTS(sort, var)	\
  ___INTERNAL_##sort##_SYSCALL_OUT_CONSTRAINTS (var)


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_0(name, nonexistent)  \
  [num] "r" ((void *) name)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_1(name, a1)           \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_0 (name,),                  \
    [arg1] "r" ((void *) (a1))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_2(name, a1, a2)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_1 (name, a1),               \
    [arg2] "r" ((void *) (a2))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_3(name, a1, a2, a3)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_2 (name, a1, a2),               \
    [arg3] "r" ((void *) (a3))


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_4(name, a1, a2, a3, a4)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_3 (name, a1, a2, a3),               \
    [arg4] "r" ((void *) (a4))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_5(name, a1, a2, a3, a4, a5)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_4 (name, a1, a2, a3, a4),		\
    [arg5] "r" ((void *) (a5))

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_6(name, a1, a2, a3, a4, a5, a6) \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_5 (name, a1, a2, a3, a4, a5),	\
    [arg6] "r" ((void *) (a6))


#define __INTERNAL_SYSCALL_LOAD_ARGS_0		\
  "movtq %[num], %%b[0]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_1		 \
  __INTERNAL_SYSCALL_LOAD_ARGS_0		 \
  "movtq %[arg1], %%b[2]\n\t"

# define __INTERNAL_SYSCALL_LOAD_ARGS_2		\
  __INTERNAL_SYSCALL_LOAD_ARGS_1                \
  "movtq %[arg2], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_3          \
  __INTERNAL_SYSCALL_LOAD_ARGS_2                \
  "movtq %[arg3], %%b[6]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_4          \
  __INTERNAL_SYSCALL_LOAD_ARGS_3                \
  "movtq %[arg4], %%b[8]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_5          \
  __INTERNAL_SYSCALL_LOAD_ARGS_4                \
  "movtq %[arg5], %%b[10]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_6          \
  __INTERNAL_SYSCALL_LOAD_ARGS_5                \
  "movtq %[arg6], %%b[12]\n\t"

# define __INTERNAL_SYSCALL_SYSCALL_LOAD_RES	\
  "addd 0x0, %%db[0], %[res]\n\t"

# define __INTERNAL_BOGUS_SYSCALL_SYSCALL_LOAD_RES	\
  "addd 0x0, %%db[0], %[err]\n\t"			\
  "movtq %%qb[2], %[res]\n\t"

#define __INTERNAL_SYSCALL_LOAD_RES(sort)	\
  __INTERNAL_##sort##_SYSCALL_LOAD_RES


#define __INTERNAL_SYSCALL_SYSCALL_RETURN_RES(r)	\
  (r)

#define __INTERNAL_BOGUS_SYSCALL_SYSCALL_RETURN_RES(r)	\
  (__err == 0 ? (r) : (void *) (-1L * (long) r))

#define __INTERNAL_SYSCALL_RETURN_RES(sort, r)	\
  __INTERNAL_##sort##_SYSCALL_RETURN_RES (r)


# else /* ! defined __ptr128_new_abi__  */

// # define PTR_MANGLE(var)	(var)
// # define PTR_DEMANGLE(var)	PTR_MANGLE (var)

#define ___INTERNAL_SYSCALL_SYSCALL_RES(var)	\
  register long var

#define ___INTERNAL_BOGUS_SYSCALL_SYSCALL_RES(var)	\
  register void *var

/* FIXME: when invoked via INTERNAL_SYSCALL_NCS SORT is empty. */
#define ___INTERNAL__SYSCALL_RES(var)	\
  register long var

#define __INTERNAL_SYSCALL_RES(sort, var)	\
  ___INTERNAL_##sort##_SYSCALL_RES (var)


#define __INTERNAL_SYSCALL_SYSCALL_LOAD_RES     \
  "addd 0x0, %%db[0], %[res]\n\t"

#define __INTERNAL_BOGUS_SYSCALL_SYSCALL_LOAD_RES	  \
  "addd 0x0, %%db[0], %[kind]\n\t"			  \
  "movtq %%qb[2], %[ptr]\n\t"

#define __INTERNAL_SYSCALL_LOAD_RES(sort)       \
    __INTERNAL_##sort##_SYSCALL_LOAD_RES

#define __INTERNAL_SYSCALL_OUT_CONSTRAINTS_BOGUS(e, a)	\
  [kind] "=r" (e), [ptr] "=r" (a)

#define __INTERNAL_SYSCALL_LOAD_RES_BOGUS \
  "addd 0x0, %%db[0], %[kind]\n\t"	  \
  "movtq %%qb[2], %[ptr]\n\t"

#define __GET_FUTEX_CMD(a2)			\
  (a2 & ~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME))
  

#define __FILL_FUTEX_ARR_2(a1, a2)		\
  __uaddr = (int *) a1;				\
  __op = a2;					\
  __val = 0;

/* According to `man 2 futex' for all 4-argument requests `FUTEX_WA{IT,KE}' and
   `FUTEX_{,TRY,UN}LOCK_PI' used in glibc the 4-th parameter is treated as
   TIMEOUT rather than VAL2 though it's said to be ignored for the last two
   ones along with VAL, UADDR2 and VAL3. Why aren't these requests performed
   as 2-argument ones on  glibc side then? Similarly, for `FUTEX_WAKE' it's
   said to be ignored along with UADDR2 and VAL3, which could probably make it
   possible to perform it as 3-argument one. For now unconditionally treat the
   4-th argument as a pointer.  */
#define __FILL_FUTEX_ARR_4(a1, a2, a3, a4)	\
  __uaddr = (int *) a1;				\
  __op = a2;					\
  __val = a3;					\
  __arr[0] =  (void *) a4;

/* FUTEX_WAIT_BITSET seems to be the only 6-argument request used on glibc
   side interpreting its 4-th parameter as TIMEOUT.  */
#define __FILL_FUTEX_ARR_6(a1, a2, a3, a4, a5, a6)	\
  unsigned long *__l = (unsigned long *) &__arr[0];	\
  int __cmd = __GET_FUTEX_CMD (a2);			\
  __uaddr = (int *) a1;					\
  __op = a2;						\
  __val = a3;						\
							\
  if (__cmd == FUTEX_WAIT_BITSET)			\
    __arr[0] = (void *) a4;				\
  else							\
    __l[0] = (unsigned long) a4;			\
							\
  __arr[1] = a5;					\
  __l[4] = a6



#define __INTERNAL_FUTEX_SYSCALL(trapnum, name, err, nr, args...)	\
  ({									\
    __INTERNAL_SYSCALL_RES (SYSCALL, syscall_res);			\
									\
    int *__uaddr;							\
    int __op;								\
    int __val;								\
    void *__arr[3];							\
									\
    __FILL_FUTEX_ARR_##nr (args);					\
									\
  __asm__ __volatile__							\
    (__INTERNAL_SYSCALL_SDISP (trapnum)					\
     __INTERNAL_SYSCALL_LOAD_ARGS_PLLP_4				\
     __INTERNAL_SYSCALL_CALL						\
     __INTERNAL_SYSCALL_LOAD_RES (SYSCALL)				\
     : __INTERNAL_SYSCALL_OUT_CONSTRAINTS (SYSCALL, syscall_res)	\
     : __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLLP_4 (name, __uaddr, __op, __val, __arr) \
     : __INTERNAL_SYSCALL_CLOBBERS);					\
									\
  syscall_res;								\
  })


#define __INTERNAL_SYSCALL_CUSTOM(arg_types, trapnum, name, err, nr, args...) \
  ({									\
    __INTERNAL_SYSCALL_RES (SYSCALL, syscall_res);			\
									\
  __asm__ __volatile__							\
    (__INTERNAL_SYSCALL_SDISP (trapnum)					\
     __INTERNAL_SYSCALL_LOAD_ARGS_##arg_types##_##nr			\
     __INTERNAL_SYSCALL_CALL						\
     __INTERNAL_SYSCALL_LOAD_RES (SYSCALL)				\
     : __INTERNAL_SYSCALL_OUT_CONSTRAINTS (SYSCALL, syscall_res)	\
     : __INTERNAL_SYSCALL_IN_CONSTRAINTS_##arg_types##_##nr (name, args) \
     : __INTERNAL_SYSCALL_CLOBBERS);					\
									\
  syscall_res;								\
  })

#define __INTERNAL_BOGUS_SYSCALL_CUSTOM(arg_types, trapnum, name, err, nr, args...) \
  ({									\
    __INTERNAL_SYSCALL_RES (BOGUS_SYSCALL, syscall_res);		\
    long __err;								\
    void *__ap;								\
									\
    __asm__ __volatile__						\
      (__INTERNAL_SYSCALL_SDISP (trapnum)				\
       __INTERNAL_SYSCALL_LOAD_ARGS_##arg_types##_##nr			\
       __INTERNAL_SYSCALL_CALL						\
       __INTERNAL_SYSCALL_LOAD_RES_BOGUS				\
       : __INTERNAL_SYSCALL_OUT_CONSTRAINTS_BOGUS (__err, __ap)		\
       : __INTERNAL_SYSCALL_IN_CONSTRAINTS_##arg_types##_##nr (name, args) \
       : __INTERNAL_SYSCALL_CLOBBERS);					\
									\
    if (__err == 0)							\
      syscall_res = __ap;						\
    else								\
      syscall_res = (void *) (-1L * (long) __ap);			\
									\
    syscall_res;							\
  })

#define __INTERNAL_SYSCALL_empty(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (empty, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_getpid_0 __INTERNAL_SYSCALL_empty
#define __INTERNAL_SYSCALL_gettid_0 __INTERNAL_SYSCALL_empty
/* Note that `__NR_pause' hasn't been supported in PM yet.  */
#define __INTERNAL_SYSCALL_pause_0 __INTERNAL_SYSCALL_empty
#define __INTERNAL_SYSCALL_fork_0 __INTERNAL_SYSCALL_empty
#define __INTERNAL_SYSCALL_vfork_0 __INTERNAL_SYSCALL_empty
#define __INTERNAL_SYSCALL_sched_yield_0 __INTERNAL_SYSCALL_empty



#define __INTERNAL_SYSCALL_L(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (L, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_exit_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_exit_group_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_close_1 __INTERNAL_SYSCALL_L
/* FIXME: neither of `set{g,u}id{,32} is actually supported in PM.  */
#define __INTERNAL_SYSCALL_setgid_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_setgid32_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_setuid_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_setuid32_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_fdatasync_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_fsync_1 __INTERNAL_SYSCALL_L

/* Not actually supported in PM.  */
#define __INTERNAL_SYSCALL_timer_delete_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_timer_getoverrun_1 __INTERNAL_SYSCALL_L
#define __INTERNAL_SYSCALL_sched_getscheduler_1 __INTERNAL_SYSCALL_L

#define __INTERNAL_BOGUS_SYSCALL_L(trapnum, name, err, nr, args...)	\
  __INTERNAL_BOGUS_SYSCALL_CUSTOM (L, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_get_mem_1 __INTERNAL_BOGUS_SYSCALL_L


#define __INTERNAL_SYSCALL_P(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (P, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_uname_1 __INTERNAL_SYSCALL_P
#define __INTERNAL_SYSCALL_free_mem_1 __INTERNAL_SYSCALL_P
#define __INTERNAL_SYSCALL_times_1 __INTERNAL_SYSCALL_P

/* Funnily enough, this syscall seems to be supported in PM. See if it can
   really help in supporting TLS.  */
#define __INTERNAL_SYSCALL_set_tid_address_1 __INTERNAL_SYSCALL_P

/* FIXME: I'm not sure if brk currently makes any sense in PM.  */
#define __INTERNAL_SYSCALL_brk_1 __INTERNAL_SYSCALL_P

#define __INTERNAL_SYSCALL_mq_unlink_1 __INTERNAL_SYSCALL_P
#define __INTERNAL_SYSCALL__sysctl_1 __INTERNAL_SYSCALL_P
#define __INTERNAL_SYSCALL_mount_1 __INTERNAL_SYSCALL_P

#define __INTERNAL_SYSCALL_LL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_kill_2 __INTERNAL_SYSCALL_LL

/* `ftruncate{,64} ()' are not actually supported in PM.  */
#define __INTERNAL_SYSCALL_ftruncate64_2 __INTERNAL_SYSCALL_LL

/* Define 2-argument variant of futex this way quite formally.  */
#define __INTERNAL_SYSCALL_futex_2 __INTERNAL_FUTEX_SYSCALL

#define __INTERNAL_SYSCALL_getpriority_2 __INTERNAL_SYSCALL_LL
#define __INTERNAL_SYSCALL_setregid32_2 __INTERNAL_SYSCALL_LL
#define __INTERNAL_SYSCALL_setreuid32_2 __INTERNAL_SYSCALL_LL


#define __INTERNAL_SYSCALL_LP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_clock_gettime_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_clock_settime_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_clock_getres_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_getgroups_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_setgroups_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_fstat_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_mq_notify_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_timer_gettime_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_sched_getparam_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_socketcall_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_ustat_2 __INTERNAL_SYSCALL_LP
#define __INTERNAL_SYSCALL_ipc_2 __INTERNAL_SYSCALL_LP




#define __INTERNAL_SYSCALL_PL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (PL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_getcwd_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_access_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_munmap_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_rt_sigsuspend_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_rt_sigpending_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_open_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_creat_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_set_robust_list_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_truncate64_2 __INTERNAL_SYSCALL_PL
#define __INTERNAL_SYSCALL_mlock_2 __INTERNAL_SYSCALL_PL

#define __INTERNAL_SYSCALL_PP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (PP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_rename_2 __INTERNAL_SYSCALL_PP
#define __INTERNAL_SYSCALL_gettimeofday_2 __INTERNAL_SYSCALL_PP
#define __INTERNAL_SYSCALL_nanosleep_2 __INTERNAL_SYSCALL_PP
#define __INTERNAL_SYSCALL_newuselib_2 __INTERNAL_SYSCALL_PP
#define __INTERNAL_SYSCALL_stat_2 __INTERNAL_SYSCALL_PP
#define __INTERNAL_SYSCALL_lstat_2 __INTERNAL_SYSCALL_PP
#define __INTERNAL_SYSCALL_utimes_2 __INTERNAL_SYSCALL_PP


#define __INTERNAL_SYSCALL_LLL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_tgkill_3 __INTERNAL_SYSCALL_LLL
#define __INTERNAL_SYSCALL_setresgid_3 __INTERNAL_SYSCALL_LLL
#define __INTERNAL_SYSCALL_setresgid32_3 __INTERNAL_SYSCALL_LLL
#define __INTERNAL_SYSCALL_setresuid_3 __INTERNAL_SYSCALL_LLL
#define __INTERNAL_SYSCALL_setresuid32_3 __INTERNAL_SYSCALL_LLL
#define __INTERNAL_SYSCALL_readahead_3 __INTERNAL_SYSCALL_LLL

/* Note that man pages document this syscall as a 4-argument one, but
   only three parameters are passed to the kernel in the implementation
   of `reboot ()'.  */
#define __INTERNAL_SYSCALL_reboot_3 __INTERNAL_SYSCALL_LLL



#define __INTERNAL_SYSCALL_LLP(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LLP, trapnum, name, err, nr, args)

/* Note that this syscall isn't actually supported in PM in fact.  */
#define __INTERNAL_SYSCALL_rt_sigqueueinfo_3 __INTERNAL_SYSCALL_LLP
#define __INTERNAL_SYSCALL_sched_setaffinity_3 __INTERNAL_SYSCALL_LLP
#define __INTERNAL_SYSCALL_sched_getaffinity_3 __INTERNAL_SYSCALL_LLP

/* fcntl64 is used in such a variant in glibc sources.  */
#define __INTERNAL_SYSCALL_fcntl64_3 __INTERNAL_SYSCALL_LLP

/* FIXME: see if there are other variants of ioctl usage throughout glibc.  */
#define __INTERNAL_SYSCALL_ioctl_3 __INTERNAL_SYSCALL_LLP

#define __INTERNAL_SYSCALL_sched_setscheduler_3 __INTERNAL_SYSCALL_LLP



#define __INTERNAL_SYSCALL_LPL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LPL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_read_3 __INTERNAL_SYSCALL_LPL
#define __INTERNAL_SYSCALL_write_3 __INTERNAL_SYSCALL_LPL
#define __INTERNAL_SYSCALL_writev_3 __INTERNAL_SYSCALL_LPL
#define __INTERNAL_SYSCALL_getdents_3 __INTERNAL_SYSCALL_LPL
#define __INTERNAL_SYSCALL_getdents64_3 __INTERNAL_SYSCALL_LPL
#define __INTERNAL_SYSCALL_waitpid_3 __INTERNAL_SYSCALL_LPL
#define __INTERNAL_SYSCALL_openat_3 __INTERNAL_SYSCALL_LPL
/* faccessat seems to be used in 3-argument variant in glibc sources.  */
#define __INTERNAL_SYSCALL_faccessat_3 __INTERNAL_SYSCALL_LPL

/* FIXME: why is fchmodat documented as a 4-argument syscall in man pages?  */
#define __INTERNAL_SYSCALL_fchmodat_3 __INTERNAL_SYSCALL_LPL

#define __INTERNAL_SYSCALL_open_by_handle_at_3 __INTERNAL_SYSCALL_LPL
#define __INTERNAL_SYSCALL_readv_3 __INTERNAL_SYSCALL_LPL


#define __INTERNAL_SYSCALL_LPP(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LPP, trapnum, name, err, nr, args)

/* FIXME: this is a 3-argument variant of clone taking flags, child stack
   descriptor and some opaque pointer which may probably eliminate the need
   for 5-argument variant of this syscall in PM. I doubt if it's going to
   be workable without additional efforts.  */
#define __INTERNAL_SYSCALL_clone_3 __INTERNAL_SYSCALL_LPP
#define __INTERNAL_SYSCALL_timer_create_3 __INTERNAL_SYSCALL_LPP
#define __INTERNAL_SYSCALL_futimesat_3 __INTERNAL_SYSCALL_LPP

/* Note that in PM I've already migrated to our own 3-argument implementation
   of select.  */
#define __INTERNAL_SYSCALL_select_3 __INTERNAL_SYSCALL_LPP



#define __INTERNAL_SYSCALL_PLL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (PLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_open_3 __INTERNAL_SYSCALL_PLL
/* I doubt if madvise makes sense in PM.  */
#define __INTERNAL_SYSCALL_madvise_3 __INTERNAL_SYSCALL_PLL
#define __INTERNAL_SYSCALL_mknod_3 __INTERNAL_SYSCALL_PLL
#define __INTERNAL_SYSCALL_poll_3 __INTERNAL_SYSCALL_PLL
#define __INTERNAL_SYSCALL_msync_3 __INTERNAL_SYSCALL_PLL
#define __INTERNAL_SYSCALL_getrandom_3 __INTERNAL_SYSCALL_PLL
#define __INTERNAL_SYSCALL_mlock2_3 __INTERNAL_SYSCALL_PLL

#define __INTERNAL_SYSCALL_PPL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (PPL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_readlink_3 __INTERNAL_SYSCALL_PPL


#define __INTERNAL_SYSCALL_PPP(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (PPP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_getcpu_3 __INTERNAL_SYSCALL_PPP
#define __INTERNAL_SYSCALL_execve_3 __INTERNAL_SYSCALL_PPP

#define __INTERNAL_SYSCALL_LLLL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LLLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_fallocate_4 __INTERNAL_SYSCALL_LLLL

/* Passing arguments to `futex' is rather weird in PM. Until you find out
   how all those `INLINE_SYSCALL (futex, nr_args, . . . )' should be adapted
   to it, stupidly implement various variants of `__INTERNAL_SYSCALL_futex_
   nr_args ()' just to proceed with building glibc.  */
#define __INTERNAL_SYSCALL_futex_4 __INTERNAL_FUTEX_SYSCALL

#define __INTERNAL_SYSCALL_fadvise64_64_4 __INTERNAL_SYSCALL_LLLL
#define __INTERNAL_SYSCALL_sync_file_range_4 __INTERNAL_SYSCALL_LLLL
#define __INTERNAL_SYSCALL_tee_4 __INTERNAL_SYSCALL_LLLL

#define __INTERNAL_SYSCALL_LLLP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LLLP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_rt_tgsigqueueinfo_4 __INTERNAL_SYSCALL_LLLP



#define __INTERNAL_SYSCALL_LLPL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LLPL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_LLPP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LLPP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_timersettime_4 __INTERNAL_SYSCALL_LLPP
#define __INTERNAL_SYSCALL_clock_nanosleep_4 __INTERNAL_SYSCALL_LLPP
#define __INTERNAL_SYSCALL_prlimit64_4 __INTERNAL_SYSCALL_LLPP

/* Hardly can I imagine if implementing `ptrace ()' this way in PM makes
   sense.  */
#define __INTERNAL_SYSCALL_ptrace_4 __INTERNAL_SYSCALL_LLPP

#define __INTERNAL_SYSCALL_timer_settime_4 __INTERNAL_SYSCALL_LLPP


#define __INTERNAL_SYSCALL_LPLL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_pread64_4 __INTERNAL_SYSCALL_LPLL
#define __INTERNAL_SYSCALL_pwrite64_4 __INTERNAL_SYSCALL_LPLL
#define __INTERNAL_SYSCALL_openat_4 __INTERNAL_SYSCALL_LPLL
#define __INTERNAL_SYSCALL_mknodat_4 __INTERNAL_SYSCALL_LPLL
#define __INTERNAL_SYSCALL_sendmmsg_4 __INTERNAL_SYSCALL_LPLL
#define __INTERNAL_SYSCALL_signalfd4_4 __INTERNAL_SYSCALL_LPLL
#define __INTERNAL_SYSCALL_vmsplice_4 __INTERNAL_SYSCALL_LPLL
#define __INTERNAL_SYSCALL_epoll_wait_4 __INTERNAL_SYSCALL_LPLL






#define __INTERNAL_SYSCALL_LPLP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPLP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_renameat_4 __INTERNAL_SYSCALL_LPLP
/* FIXME: for `__NR_wait4' they swapped the 3-rd and the 4-th arguments in
   the Kernel in fact.  */
#define __INTERNAL_SYSCALL_wait4_4 __INTERNAL_SYSCALL_LPLP



#define __INTERNAL_SYSCALL_LPPL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPPL, trapnum, name, err, nr, args)

/* NOTE: our Kernel isn't likely to make use of the fourth parameter to
   `__NR_rt_sigprocmask' in Protected Mode at present.  */
#define __INTERNAL_SYSCALL_rt_sigprocmask_4 __INTERNAL_SYSCALL_LPPL
#define __INTERNAL_SYSCALL_rt_sigaction_4 __INTERNAL_SYSCALL_LPPL
#define __INTERNAL_SYSCALL_rt_sigaction_ex_4 __INTERNAL_SYSCALL_LPPL
/* FIXME: newfstatat isn't actually supported in PM.  */
#define __INTERNAL_SYSCALL_newfstatat_4 __INTERNAL_SYSCALL_LPPL

#define __INTERNAL_SYSCALL_utimensat_4 __INTERNAL_SYSCALL_LPPL

#define __INTERNAL_SYSCALL_LPPP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPPP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_pselect6_4 __INTERNAL_SYSCALL_LPPP
#define __INTERNAL_SYSCALL_ppoll_4 __INTERNAL_SYSCALL_LPPP


#define __INTERNAL_SYSCALL_PLLP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (PLLP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_mq_open_4 __INTERNAL_SYSCALL_PLLP

#define __INTERNAL_SYSCALL_PLLL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (PLLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_pkey_mprotect_4 __INTERNAL_SYSCALL_PLLL


#define __INTERNAL_SYSCALL_PPPL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (PPPL, trapnum, name, err, nr, args)

/* NOTE: our Kernel is likely to pass the fourth parameter on %r1 to
   `__NR_rt_sigtimedwait' syscall in Protected Mode at present.  */
#define __INTERNAL_SYSCALL_rt_sigtimedwait_4 __INTERNAL_SYSCALL_PPPL


#define __INTERNAL_SYSCALL_LLLLP(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LLLLP, trapnum, name, err, nr, args)


#define __INTERNAL_SYSCALL_LLLPP(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LLLPP, trapnum, name, err, nr, args)

/* FIXME: waitid isn't actually supported in PM yet.  */
#define __INTERNAL_SYSCALL_waitid_5 __INTERNAL_SYSCALL_LLLPP



#define __INTERNAL_SYSCALL_LLLPL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LLLPL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL__llseek_5 __INTERNAL_SYSCALL_LLLPL

#define __INTERNAL_SYSCALL_LPLLL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPLLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_preadv64_5 __INTERNAL_SYSCALL_LPLLL
#define __INTERNAL_SYSCALL_pwritev64_5 __INTERNAL_SYSCALL_LPLLL

#define __INTERNAL_SYSCALL_LPLLP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPLLP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_mq_timedsend_5 __INTERNAL_SYSCALL_LPLLP
#define __INTERNAL_SYSCALL_recvmmsg_5 __INTERNAL_SYSCALL_LPLLP

#define __INTERNAL_SYSCALL_LPLPP(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPLPP, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_mq_timedreceive_5 __INTERNAL_SYSCALL_LPLPP

/* Stupidly implement 5-argument variant of ppoll this way just to proceed
   with making glibc.  */
#define __INTERNAL_SYSCALL_ppoll_5 __INTERNAL_SYSCALL_LPLPP

#define __INTERNAL_SYSCALL_LPPPL(trapnum, name, err, nr, args...)		\
  __INTERNAL_SYSCALL_CUSTOM (LPPPL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_execveat_5 __INTERNAL_SYSCALL_LPPPL




#define __INTERNAL_BOGUS_SYSCALL_LLLLLL(trapnum, name, err, nr, args...)	\
  __INTERNAL_BOGUS_SYSCALL_CUSTOM (LLLLLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_mmap_6 __INTERNAL_BOGUS_SYSCALL_LLLLLL
#define __INTERNAL_SYSCALL_mmap2_6 __INTERNAL_BOGUS_SYSCALL_LLLLLL


#define __INTERNAL_SYSCALL_LLLLLL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LLLLLL, trapnum, name, err, nr, args)

/* See above on futex syscall . . .  */
#define __INTERNAL_SYSCALL_futex_6 __INTERNAL_FUTEX_SYSCALL

/* A fake implementation of copy_file_range.  */
#define __INTERNAL_SYSCALL_copy_file_range_6 __INTERNAL_SYSCALL_LLLLLL


#define __INTERNAL_SYSCALL_LPLLLL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LPLLLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_pwritev64v2_6 __INTERNAL_SYSCALL_LPLLLL
#define __INTERNAL_SYSCALL_preadv64v2_6 __INTERNAL_SYSCALL_LPLLLL



#define __INTERNAL_SYSCALL_LLLLPL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LLLLPL, trapnum, name, err, nr, args)

/* FIXME: ipc obtains its parameters in a rather weird way in PM. It's to be
   revisited and adapted to PM in fact.  */
#define __INTERNAL_SYSCALL_ipc_6 __INTERNAL_SYSCALL_LLLLPL



#define __INTERNAL_SYSCALL_LPLLPL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LPLLPL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_epoll_pwait_6 __INTERNAL_SYSCALL_LPLLPL


#define __INTERNAL_SYSCALL_LPLPLL(trapnum, name, err, nr, args...)	\
  __INTERNAL_SYSCALL_CUSTOM (LPLPLL, trapnum, name, err, nr, args)

#define __INTERNAL_SYSCALL_splice_6 __INTERNAL_SYSCALL_LPLPLL



#define INTERNAL_SYSCALL(name, err, nr, args...) __INTERNAL_SYSCALL_##name##_##nr (__SYSCALL_TRAPNUM, __NR_##name, err, nr, args)
#define INTERNAL_BOGUS_SYSCALL(name, err, nr, args...) INTERNAL_SYSCALL (name, err, nr, args)



#define __INTERNAL_SYSCALL_LOAD_ARGS_empty_0	\
  "addd 0x0, %[num], %%b[0]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_empty_0(name, nonexistent) \
  [num] "ri" (name)


#define __INTERNAL_SYSCALL_LOAD_ARGS_L_0	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_L_1	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd,sm 0x0, %[arg1], %%b[1]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_L_2	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_L_3	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_L_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_L_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_L_6	""


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_L_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_L_1(name, a1)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1)

/* FIXME: unfortunately I can't do just with a single `__INTERNAL_SYSCALL_IN_
   CONSTRAINTS_L ()' since that would result in preprocessor error when it's
   substituted with a wrong number of arguments despite the fact that the result
   of this substitution will have no chance of being executed at all . . .  */
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_L_2(name, a1, a2)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_L_3(name, a1, a2, a3)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_L_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_L_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_L_6(name, a1, a2, a3, a4, a5, a6)


#define __INTERNAL_SYSCALL_LOAD_ARGS_P_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_P_1	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_P_2	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_P_3	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_P_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_P_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_P_6	""


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_P_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_P_1(name, a1)	\
  [num] "ri" (name), [arg1] "ri" (a1)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_P_2(name, a1, a2)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_P_3(name, a1, a2, a3)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_P_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_P_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_P_6(name, a1, a2, a3, a4, a5, a6)

#define __INTERNAL_SYSCALL_LOAD_ARGS_LL_2		\
  "addd 0x0, %[num], %%b[0]\n\t"			\
  "addd 0x0, %[arg1], %%b[1]\n\t"			\
  "addd 0x0, %[arg2], %%b[2]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LL_2(name, a1, a2)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1), [arg2] "ri" ((long) a2)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_2		\
  "addd 0x0, %[num], %%b[0]\n\t"			\
  "addd 0x0, %[arg1], %%b[1]\n\t"			\
  "movtq %[arg2], %%b[2]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_2(name, a1, a2)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1), [arg2] "ri" (a2)



#define __INTERNAL_SYSCALL_LOAD_ARGS_PL_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PL_1	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_PL_2	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "addd 0x0, %[arg2], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_PL_3	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PL_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PL_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PL_6	""


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PL_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PL_1(name, a1)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PL_2(name, a1, a2)	\
  [num] "ri" (name), [arg1] "ri" (a1), [arg2] "ri" ((long) a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PL_3(name, a1, a2, a3)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PL_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PL_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PL_6(name, a1, a2, a3, a4, a5, a6)



#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_1	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_2	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_3	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LP_6	""


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_1(name, a1)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_2(name, a1, a2)		\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_3(name, a1, a2, a3)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LP_6(name, a1, a2, a3, a4, a5, a6)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LPL_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPL_1	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPL_2	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_LPL_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_LPL_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPL_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPL_6	""

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPL_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPL_1(name, a1)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPL_2(name, a1, a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPL_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPL_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPL_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPL_6(name, a1, a2, a3, a4, a5, a6)

#define __INTERNAL_SYSCALL_LOAD_ARGS_LPP_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "movtq %[arg3], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPP_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" (a3)

#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLL_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"		\
  "addd 0x0, %[arg4], %%b[4]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLL_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),			\
    [arg4] "ri" ((long) a4)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLP_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"		\
  "movtq %[arg4], %%b[4]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLP_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),			\
    [arg4] "ri" (a4)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLPL_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "movtq %[arg3], %%b[4]\n\t"			\
  "addd 0x0, %[arg4], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLPL_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" (a3),				\
    [arg4] "ri" ((long) a4)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLPP_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "movtq %[arg3], %%b[4]\n\t"			\
  "movtq %[arg4], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLPP_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" (a3),				\
    [arg4] "ri" (a4)



#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLP_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "movtq %[arg4], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLP_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3),				\
    [arg4] "ri" (a4)





#define __INTERNAL_SYSCALL_LOAD_ARGS_LPPL_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "movtq %[arg3], %%b[4]\n\t"			\
  "addd 0x0, %[arg4], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPPL_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" (a3),					\
    [arg4] "ri" ((long) a4)

#define __INTERNAL_SYSCALL_LOAD_ARGS_LPPP_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "movtq %[arg3], %%b[4]\n\t"			\
  "movtq %[arg4], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPPP_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" (a3),					\
    [arg4] "ri" (a4)



#define __INTERNAL_SYSCALL_LOAD_ARGS_PLLP_4		\
  "addd 0x0, %[num], %%b[0]\n\t"			\
  "movtq %[arg1], %%b[2]\n\t"				\
  "addd 0x0, %[arg2], %%b[4]\n\t"			\
  "addd 0x0, %[arg3], %%b[5]\n\t"			\
  "movtq %[arg4], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLLP_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" (a1),					\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),			\
    [arg4] "ri" (a4)



/* Note that the fourth argument is passed on %b[1] in fact. This is inspired
   by PM implementation by `__NR_rt_sigtimedwait'.  */
#define __INTERNAL_SYSCALL_LOAD_ARGS_PPPL_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg4], %%b[1]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "movtq %[arg2], %%b[4]\n\t"			\
  "movtq %[arg3], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPPL_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" (a1),					\
    [arg2] "ri" (a2), [arg3] "ri" (a3),					\
    [arg4] "ri" ((long) a4)



#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLL_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLL_1	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLL_2	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLL_3	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLL_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "addd 0x0, %[arg4], %%b[5]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLL_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLL_6	""

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLL_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLL_1(name, a1)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLL_2(name, a1, a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLL_3(name, a1, a2, a3)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLL_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3),				\
    [arg4] "ri" ((long) a4)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLL_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLL_6(name, a1, a2, a3, a4, a5, a6)

#define __INTERNAL_SYSCALL_LOAD_ARGS_PLLL_4	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "addd 0x0, %[arg2], %%b[4]\n\t"		\
  "addd 0x0, %[arg3], %%b[5]\n\t"		\
  "addd 0x0, %[arg4], %%b[6]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLLL_4(name, a1, a2, a3, a4)	\
  [num] "ri" (name), [arg1] "ri" (a1),					\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),		\
    [arg4] "ri" ((long) a4)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLL_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"			\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"

/* FIXME: this way I make it possible to use the generic implementation of
   `__INTERNAL_SYSCALL_NCS ()' without the need to have PM-specific. Here I
   rely on the fact that in glibc sources it's always invoked with 3 long
   arguments. */
#define __INTERNAL_SYSCALL_LOAD_ARGS_3	__INTERNAL_SYSCALL_LOAD_ARGS_LLL_3

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLL_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLP_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"			\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "movtq %[arg3], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLP_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" (a3)



#define __INTERNAL_SYSCALL_LOAD_ARGS_PLL_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PLL_1	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PLL_2	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_PLL_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "addd 0x0, %[arg2], %%b[4]\n\t"		\
  "addd 0x0, %[arg3], %%b[5]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_PLL_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PLL_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PLL_6	""

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLL_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLL_1(name, a1)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLL_2(name, a1, a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLL_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" (a1),					\
    [arg2] "ri" ((long) (a2)), [arg3] "ri" ((long) a3)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLL_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLL_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PLL_6(name, a1, a2, a3, a4, a5, a6)

#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "movtq %[arg2], %%b[4]\n\t"			\
  "addd 0x0, %[arg3], %%b[6]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" (a1),					\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3)



#define __INTERNAL_SYSCALL_LOAD_ARGS_PPP_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "movtq %[arg2], %%b[4]\n\t"			\
  "movtq %[arg3], %%b[6]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPP_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" (a1),					\
    [arg2] "ri" (a2), [arg3] "ri" (a3)



#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPL_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPL_1	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPL_2	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPL_3	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPL_4	""


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLP_5	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"		\
  "addd 0x0, %[arg4], %%b[4]\n\t"		\
  "movtq %[arg5], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLP_5(name, a1, a2, a3, a4, a5) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),			\
    [arg4] "ri" ((long) a4), [arg5] "ri" (a5)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPL_5	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"		\
  "movtq %[arg4], %%b[4]\n\t"			\
  "addd 0x0, %[arg5], %%b[6]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPL_6	""

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPL_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPL_1(name, a1)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPL_2(name, a1, a2)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPL_3(name, a1, a2, a3)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPL_4(name, a1, a2, a3, a4)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPL_5(name, a1, a2, a3, a4, a5) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),			\
    [arg4] "ri" (a4), [arg5] "ri" ((long) a5)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPL_6(name, a1, a2, a3, a4, a5, a6)


/* This syscall type is needed for waitid which is likely to accept 5 parameters
   despite the fact that its man page documents only 4. Note, however, that we
   are currently incapable of passing the fifth argument to the Kernel.
   Fortunately, it seems to be unused in Linux implementation of
   `waitid ()'.   */
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLPP_5	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"		\
  "movtq %[arg4], %%b[4]\n\t"			\
  "movtq %[arg5], %%b[6]\n\t"




#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLPP_5(name, a1, a2, a3, a4, a5) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
  [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),			\
  [arg4] "ri" (a4), [arg5] "ri" (a5)

#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLLL_5	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "addd 0x0, %[arg4], %%b[5]\n\t"		\
  "addd 0x0, %[arg5], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLLL_5(name, a1, a2, a3, a4, a5) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3),				\
    [arg4] "ri" ((long) a4), [arg5] "ri" ((long) a5)



#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLLP_5	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "addd 0x0, %[arg4], %%b[5]\n\t"		\
  "movtq %[arg5], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLLP_5(name, a1, a2, a3, a4, a5) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3),				\
    [arg4] "ri" ((long) a4), [arg5] "ri" (a5)


/* FIXME: we are currently incapable of passing the trailing pointer to the
   Kernel.  */
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLPP_5	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "movtq %[arg4], %%b[6]\n\t"




#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLPP_5(name, a1, a2, a3, a4, a5) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3),				\
    [arg4] "ri" (a4), [arg5] "ri" (a5)


/* FIXME: we are currently incapable of passing the trailing long to the
   Kernel.  */
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPPPL_5	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "movtq %[arg3], %%b[4]\n\t"			\
  "movtq %[arg4], %%b[6]\n\t"			\




#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPPPL_5(name, a1, a2, a3, a4, a5) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" (a3),					\
    [arg4] "ri" (a4), [arg5] "ri" ((long) a5)




#define __INTERNAL_SYSCALL_LOAD_ARGS_PP_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PP_1	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_PP_2	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "movtq %[arg2], %%b[4]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_PP_3	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PP_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PP_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PP_6	""

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PP_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PP_1(name, a1)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PP_2(name, a1, a2)	\
  [num] "ri" (name), [arg1] "ri" (a1), [arg2] "ri" (a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PP_3(name, a1, a2, a3)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PP_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PP_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PP_6(name, a1, a2, a3, a4, a5, a6)






#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_0	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_1	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_2	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_3	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "movtq %[arg1], %%b[2]\n\t"			\
  "movtq %[arg2], %%b[4]\n\t"			\
  "addd 0x0, %[arg3], %%b[6]\n\t"

#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_PPL_6	""

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_1(name, a1)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_2(name, a1, a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_3(name, a1, a2, a3)	\
  [num] "ri" (name), [arg1] "ri" (a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_PPL_6(name, a1, a2, a3, a4, a5, a6)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLLL_0	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLLL_1	""

#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLLL_2	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLLL_3	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLLL_4	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLLL_5	""
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLLL_6	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"		\
  "addd 0x0, %[arg4], %%b[4]\n\t"		\
  "addd 0x0, %[arg5], %%b[5]\n\t"		\
  "addd 0x0, %[arg6], %%b[6]\n\t"
  


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLLL_0(name, nonexistent)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLLL_1(name, a1)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLLL_2(name, a1, a2)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLLL_3(name, a1, a2, a3)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLLL_4(name, a1, a2, a3, a4)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLLL_5(name, a1, a2, a3, a4, a5)
#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLLL_6(name, a1, a2, a3, a4, a5, a6) \
  [num] "ri" (name), [arg1] "ri" ((long) a1), [arg2] "ri" ((long) a2),	\
    [arg3] "ri" ((long) a3), [arg4] "ri" ((long) a4),			\
    [arg5] "ri" ((long) a5), [arg6] "ri" ((long) a6)


/* Note, that we are incapable of passing all 6 parameters in fact. However,
   taking into account, that this syscall type is used only as a 6-argument
   variant of ipc, which should be adapted to PM, . . .  */
#define __INTERNAL_SYSCALL_LOAD_ARGS_LLLLPL_6	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "addd 0x0, %[arg2], %%b[2]\n\t"		\
  "addd 0x0, %[arg3], %%b[3]\n\t"		\
  "addd 0x0, %[arg4], %%b[4]\n\t"		\
  "movtq %[arg5], %%b[6]\n\t"



#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LLLLPL_6(name, a1, a2, a3, a4, a5, a6) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" ((long) a2), [arg3] "ri" ((long) a3),			\
    [arg4] "ri" ((long) a4), [arg5] "ri" (a5),				\
    [arg6] "ri" ((long) a6)


#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLLLL_6	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "addd 0x0, %[arg4], %%b[5]\n\t"		\
  "addd 0x0, %[arg5], %%b[6]\n\t"		\
  "addd 0x0, %[arg6], %%b[7]\n\t"




#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLLLL_6(name, a1, a2, a3, a4, a5, a6) \
  [num] "ri" (name), [arg1] "ri" ((long) a1),				\
    [arg2] "ri" (a2), [arg3] "ri" ((long) a3),				\
    [arg4] "ri" ((long) a4), [arg5] "ri" ((long) a5),			\
    [arg6] "ri" ((long) a6)




/* Note that we are incapable of passing the 6-th argument in fact because of
   being out of registers.  */
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLLPL_6	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "addd 0x0, %[arg4], %%b[5]\n\t"		\
  "movtq %[arg5], %%b[6]\n\t"

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLLPL_6(name, a1, a2, a3, a4, a5, a6) \
  [num] "ri" (name), [arg1] "ri" ((long) a1), [arg2] "ri" (a2),	\
    [arg3] "ri" ((long) a3), [arg4] "ri" ((long) a4),			\
    [arg5] "ri" (a5), [arg6] "ri" ((long) a6)


/* FIXME: we seem to be capable of passing only the first four arguments this
   way in fact, which means that `splice ()' is currently broken . . .  */
#define __INTERNAL_SYSCALL_LOAD_ARGS_LPLPLL_6	\
  "addd 0x0, %[num], %%b[0]\n\t"		\
  "addd 0x0, %[arg1], %%b[1]\n\t"		\
  "movtq %[arg2], %%b[2]\n\t"			\
  "addd 0x0, %[arg3], %%b[4]\n\t"		\
  "movtq %[arg4], %%b[6]\n\t"


#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_LPLPLL_6(name, a1, a2, a3, a4, a5, a6) \
  [num] "ri" (name),							\
    [arg1] "ri" ((long) a1), [arg2] "ri" (a2),				\
    [arg3] "ri" ((long) a3), [arg4] "ri" (a4),				\
    [arg5] "ri" ((long) a5), [arg6] "ri" ((long) a6)





#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_0(name, nonexistent)  \
  [num] "ri" (name)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_1(name, a1)           \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_0 (name,),                  \
    [arg1] "ri" (a1)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_2(name, a1, a2)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_1 (name, a1),               \
    [arg2] "ri" (a2)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_3(name, a1, a2, a3)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_2 (name, a1, a2),           \
    [arg3] "ri" (a3)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_4(name, a1, a2, a3, a4)       \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_3 (name, a1, a2, a3),               \
    [arg4] "ri" (a4)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_5(name, a1, a2, a3, a4, a5)   \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_4 (name, a1, a2, a3, a4),           \
    [arg5] "ri" (a5)

#define __INTERNAL_SYSCALL_IN_CONSTRAINTS_6(name, a1, a2, a3, a4, a5, a6) \
  __INTERNAL_SYSCALL_IN_CONSTRAINTS_5 (name, a1, a2, a3, a4, a5),       \
    [arg6] "ri" (a6)

# endif /* ! defined __ptr128_new_abi__  */
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
