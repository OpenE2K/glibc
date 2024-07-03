/* Copyright (C) 2000-2022 Free Software Foundation, Inc.
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

#ifndef _LINUX_SPARC_SYSDEP_H
#define _LINUX_SPARC_SYSDEP_H 1

#include <sysdeps/unix/sysdep.h>
#include <sysdeps/unix/sysv/linux/sysdep.h>
#include <sysdeps/sparc/sysdep.h>

#ifdef __ASSEMBLER__

#define	ret		retl; nop
#define	ret_NOERRNO	retl; nop
#define	ret_ERRVAL	retl; nop
#define	r0		%o0
#define	r1		%o1
#define	MOVE(x,y)	mov x, y

#else	/* __ASSEMBLER__ */

# define VDSO_NAME  "LINUX_2.6"
# define VDSO_HASH  61765110

/* List of system calls which are supported as vsyscalls.  */
# ifdef __arch64__
#  define HAVE_CLOCK_GETTIME64_VSYSCALL	"__vdso_clock_gettime"
# else
#  define HAVE_CLOCK_GETTIME_VSYSCALL	"__vdso_clock_gettime"
# endif
# define HAVE_GETTIMEOFDAY_VSYSCALL	"__vdso_gettimeofday"

#undef INTERNAL_SYSCALL
#define INTERNAL_SYSCALL(name, nr, args...) \
  internal_syscall##nr(__SYSCALL_STRING, __NR_##name, args)

#undef INTERNAL_SYSCALL_NCS
#define INTERNAL_SYSCALL_NCS(name, nr, args...) \
  internal_syscall##nr(__SYSCALL_STRING, name, args)

/* Fix #Bug #50540, Comment #5 for sparc32 only meanwhile. Leave
   `inline_syscall' macros intact for sparc64 since the corresponding
   modifications are not ready yet. Moreover, I'm not quite sure if
   they are needed for sparc64 at all.  */

# if defined __LCC__ /* && defined _LINUX_SPARC32_SYSDEP_H  */

#define internal_syscall0(string,name,dummy...)				\
({									\
  register long __o0;                                                   \
  __asm __volatile (__LOAD_ARGS_0                                       \
                    string : "=r" (__o0) :				\
                    "r" (name) :					\
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define internal_syscall1(string,name,arg1)				\
({									\
  register long __o0;                                                   \
  long __syscall_args[1];                                               \
  __syscall_args[0] = (long) (arg1);					\
  __asm __volatile (__LOAD_ARGS_1                                       \
                    string : "=r" (__o0) :				\
		    "r" (name), "r" (__syscall_args) :			\
                    __SYSCALL_CLOBBERS);                                \
__o0;                                                                   \
})

#define internal_syscall2(string,name,arg1,arg2)			\
({									\
  register long __o0;                                                   \
  long __syscall_args[2];                                               \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __asm __volatile (__LOAD_ARGS_2                                       \
                    string : "=r" (__o0) :				\
		    "r" (name), "r" (__syscall_args) :			\
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define internal_syscall3(string,name,arg1,arg2,arg3)			\
({									\
  register long __o0;                                                   \
  long __syscall_args[3];                                               \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __asm __volatile (__LOAD_ARGS_3                                       \
                    string : "=r" (__o0) :				\
                    "r" (name), "r" (__syscall_args) :			\
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define internal_syscall4(string,name,arg1,arg2,arg3,arg4)		\
({									\
  register long __o0;                                                   \
  long __syscall_args[4];                                               \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __syscall_args[3] = (long) (arg4);					\
  __asm __volatile (__LOAD_ARGS_4                                       \
                    string : "=r" (__o0) :				\
                    "r" (name), "r" (__syscall_args) :			\
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define internal_syscall5(string,name,arg1,arg2,arg3,arg4,arg5)		\
({									\
  register long __o0;                                                   \
  long __syscall_args[5];                                               \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __syscall_args[3] = (long) (arg4);					\
  __syscall_args[4] = (long) (arg5);					\
  __asm __volatile (__LOAD_ARGS_5                                       \
                    string : "=r" (__o0) :				\
		    "r" (name), "r" (__syscall_args) :			\
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define internal_syscall6(string,name,arg1,arg2,arg3,arg4,arg5,arg6) \
({									\
  register long __o0;                                                   \
  long __syscall_args[6];                                               \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __syscall_args[3] = (long) (arg4);					\
  __syscall_args[4] = (long) (arg5);					\
  __syscall_args[5] = (long) (arg6);					\
  __asm __volatile (__LOAD_ARGS_6                                       \
                    string : "=r" (__o0) :				\
                    "r" (name), "r" (__syscall_args) :			\
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define INLINE_CLONE_SYSCALL(arg1,arg2,arg3,arg4,arg5)			\
({									\
  register long __g1 = __NR_clone;                                      \
  register long __o0;                                                   \
  register long __o1;                                                   \
  long __syscall_args[5];                                               \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __syscall_args[3] = (long) (arg4);					\
  __syscall_args[4] = (long) (arg5);					\
  __asm __volatile (__LOAD_ARGS_5_FOR_CLONE                             \
                    __SYSCALL_STRING_FOR_CLONE :                        \
		    "=r" (__o0), "=r" (__o1) :				\
                    "r" (__g1), "r" (__syscall_args) :                  \
                    __SYSCALL_CLOBBERS);				\
  if (__glibc_unlikely ((unsigned long int) (__o0) > -4096UL))		\
    {                                                                   \
      __set_errno (-__o0);						\
      __o0 = -1L;                                                       \
    }                                                                   \
  else                                                                  \
    {                                                                   \
      __o0 &= (__o1 - 1);                                               \
    }                                                                   \
  __o0;                                                                 \
})

# else /* ! (defined __LCC__ && defined _LINUX_SPARC32_SYSDEP_H)  */

/* Original definitions of `internal_syscall's are left
   intact here meanwhile. It's not clear if we have a
   problem related to Bug #50540, Comment #5 here at all.  */

#define internal_syscall0(string,name,dummy...)			\
({									\
	register long int __g1 __asm__ ("g1") = (name);			\
	register long __o0 __asm__ ("o0");				\
	__asm __volatile (string : "=r" (__o0) :			\
			  "r" (__g1) :					\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define internal_syscall1(string,name,arg1)				\
({									\
	long int _arg1 = (long int) (arg1);				\
	register long int __g1 __asm__("g1") = (name);			\
	register long int  __o0 __asm__ ("o0") = _arg1;			\
	__asm __volatile (string : "=r" (__o0) :			\
			  "r" (__g1), "0" (__o0) :			\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define internal_syscall2(string,name,arg1,arg2)			\
({									\
	long int _arg1 = (long int) (arg1);				\
	long int _arg2 = (long int) (arg2);				\
	register long int __g1 __asm__("g1") = (name);			\
	register long int __o0 __asm__ ("o0") = _arg1;			\
	register long int __o1 __asm__ ("o1") = _arg2;			\
	__asm __volatile (string : "=r" (__o0) :			\
			  "r" (__g1), "0" (__o0), "r" (__o1) :		\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define internal_syscall3(string,name,arg1,arg2,arg3)			\
({									\
	long int _arg1 = (long int) (arg1);				\
	long int _arg2 = (long int) (arg2);				\
	long int _arg3 = (long int) (arg3);				\
	register long int __g1 __asm__("g1") = (name);			\
	register long int __o0 __asm__ ("o0") = _arg1;			\
	register long int __o1 __asm__ ("o1") = _arg2;			\
	register long int __o2 __asm__ ("o2") = _arg3;			\
	__asm __volatile (string : "=r" (__o0) :			\
			  "r" (__g1), "0" (__o0), "r" (__o1),		\
			  "r" (__o2) :					\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define internal_syscall4(string,name,arg1,arg2,arg3,arg4)		\
({									\
	long int _arg1 = (long int) (arg1);				\
	long int _arg2 = (long int) (arg2);				\
	long int _arg3 = (long int) (arg3);				\
	long int _arg4 = (long int) (arg4);				\
	register long int __g1 __asm__("g1") = (name);			\
	register long int __o0 __asm__ ("o0") = _arg1;			\
	register long int __o1 __asm__ ("o1") = _arg2;			\
	register long int __o2 __asm__ ("o2") = _arg3;			\
	register long int __o3 __asm__ ("o3") = _arg4;			\
	__asm __volatile (string : "=r" (__o0) :			\
			  "r" (__g1), "0" (__o0), "r" (__o1),		\
			  "r" (__o2), "r" (__o3) :			\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define internal_syscall5(string,name,arg1,arg2,arg3,arg4,arg5)		\
({									\
	long int _arg1 = (long int) (arg1);				\
	long int _arg2 = (long int) (arg2);				\
	long int _arg3 = (long int) (arg3);				\
	long int _arg4 = (long int) (arg4);				\
	long int _arg5 = (long int) (arg5);				\
	register long int __g1 __asm__("g1") = (name);			\
	register long int __o0 __asm__ ("o0") = _arg1;			\
	register long int __o1 __asm__ ("o1") = _arg2;			\
	register long int __o2 __asm__ ("o2") = _arg3;			\
	register long int __o3 __asm__ ("o3") = _arg4;			\
	register long int __o4 __asm__ ("o4") = _arg5;			\
	__asm __volatile (string : "=r" (__o0) :			\
			  "r" (__g1), "0" (__o0), "r" (__o1),		\
			  "r" (__o2), "r" (__o3), "r" (__o4) :		\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define internal_syscall6(string,name,arg1,arg2,arg3,arg4,arg5,arg6)	\
({									\
	long int _arg1 = (long int) (arg1);				\
	long int _arg2 = (long int) (arg2);				\
	long int _arg3 = (long int) (arg3);				\
	long int _arg4 = (long int) (arg4);				\
	long int _arg5 = (long int) (arg5);				\
	long int _arg6 = (long int) (arg6);				\
	register long int __g1 __asm__("g1") = (name);			\
	register long int __o0 __asm__ ("o0") = _arg1;			\
	register long int __o1 __asm__ ("o1") = _arg2;			\
	register long int __o2 __asm__ ("o2") = _arg3;			\
	register long int __o3 __asm__ ("o3") = _arg4;			\
	register long int __o4 __asm__ ("o4") = _arg5;			\
	register long int __o5 __asm__ ("o5") = _arg6;			\
	__asm __volatile (string : "=r" (__o0) :			\
			  "r" (__g1), "0" (__o0), "r" (__o1),		\
			  "r" (__o2), "r" (__o3), "r" (__o4),		\
			  "r" (__o5) :					\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define INLINE_CLONE_SYSCALL(arg1,arg2,arg3,arg4,arg5)			\
({									\
	long int _arg1 = (long int) (arg1);				\
	long int _arg2 = (long int) (arg2);				\
	long int _arg3 = (long int) (arg3);				\
	long int _arg4 = (long int) (arg4);				\
	long int _arg5 = (long int) (arg5);				\
	register long int __o0 __asm__ ("o0") = _arg1;			\
	register long int __o1 __asm__ ("o1") = _arg2;			\
	register long int __o2 __asm__ ("o2") = _arg3;			\
	register long int __o3 __asm__ ("o3") = _arg4;			\
	register long int __o4 __asm__ ("o4") = _arg5;			\
	register long int __g1 __asm__ ("g1") = __NR_clone;		\
	__asm __volatile (__SYSCALL_STRING :				\
			  "=r" (__o0), "=r" (__o1) :			\
			  "r" (__g1), "0" (__o0), "1" (__o1),		\
			  "r" (__o2), "r" (__o3), "r" (__o4) :		\
			  __SYSCALL_CLOBBERS);				\
	if (__glibc_unlikely ((unsigned long int) (__o0) > -4096UL))	\
	  {		     			       		   	\
	    __set_errno (-__o0);					\
	    __o0 = -1L;			    				\
	  } 	      							\
	else								\
	  { 	      							\
	    __o0 &= (__o1 - 1);						\
	  } 	    	    						\
	__o0;								\
})

# endif /* ! (defined __LCC__ && defined _LINUX_SPARC32_SYSDEP_H)  */

#endif	/* __ASSEMBLER__ */


#if defined __LCC__
/* This attribute is used to mark functions which shouldn't be
   inlined when compiling with `-fwhole' option (see Bug #61026).
   I wonder if this is going to protect us from inlining their
   numerous aliases (versioned symbols and so on . . .). Should
   they be explicitly marked with this attribute or not? This
   depends on our `-fwhole' implementation which nobody is aware
   of.  */
# define ATTRIBUTE_NOINLINE_WHOLE __attribute__ ((noinline))
#else /* __LCC__  */
# define ATTRIBUTE_NOINLINE_WHOLE
#endif /* __LCC__  */

#endif /* _LINUX_SPARC_SYSDEP_H */
