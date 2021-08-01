/* Copyright (C) 2000-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 2000.

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

#define INTERNAL_VSYSCALL_CALL(funcptr, err, nr, args...)		\
  ({									\
    long _ret = funcptr (args);						\
    err = ((unsigned long) (_ret) >= (unsigned long) -4095L);		\
    _ret;								\
  })

/* List of system calls which are supported as vsyscalls.  */
# define HAVE_CLOCK_GETTIME_VSYSCALL	1
# define HAVE_GETTIMEOFDAY_VSYSCALL	1

#undef INLINE_SYSCALL
#define INLINE_SYSCALL(name, nr, args...) 				\
({	INTERNAL_SYSCALL_DECL(err);  					\
	unsigned long resultvar = INTERNAL_SYSCALL(name, err, nr, args);\
	if (INTERNAL_SYSCALL_ERROR_P (resultvar, err))			\
	  {		     			       		   	\
	    __set_errno (INTERNAL_SYSCALL_ERRNO (resultvar, err));	\
	    resultvar = (unsigned long) -1;				\
	  } 	      							\
	(long) resultvar;						\
})

#undef INTERNAL_SYSCALL_DECL

# if 1 //def _LINUX_SPARC32_SYSDEP_H

#define INTERNAL_SYSCALL_DECL(err) \
	register long err;

# else /* _LINUX_SPARC32_SYSDEP_H  */

#define INTERNAL_SYSCALL_DECL(err) \
	register long err __asm__("g1");

# endif /* _LINUX_SPARC32_SYSDEP_H  */


#undef INTERNAL_SYSCALL
#define INTERNAL_SYSCALL(name, err, nr, args...) \
  inline_syscall##nr(__SYSCALL_STRING, err, __NR_##name, args)

#undef INTERNAL_SYSCALL_NCS
#define INTERNAL_SYSCALL_NCS(name, err, nr, args...) \
  inline_syscall##nr(__SYSCALL_STRING, err, name, args)

#undef INTERNAL_SYSCALL_ERROR_P
#define INTERNAL_SYSCALL_ERROR_P(val, err) \
  ((void) (val), __builtin_expect((err) != 0, 0))

#undef INTERNAL_SYSCALL_ERRNO
#define INTERNAL_SYSCALL_ERRNO(val, err)	(-(val))

/* Fix #Bug #50540, Comment #5 for sparc32 only meanwhile. Leave
   `inline_syscall' macros intact for sparc64 since the corresponding
   modifications are not ready yet. Moreover, I'm not quite sure if
   they are needed for sparc64 at all.  */

# if 1 //def _LINUX_SPARC32_SYSDEP_H

#define inline_syscall0(string,err,name,dummy...)                       \
({									\
  register long __o0;                                                   \
  err = name;                                                           \
  __asm __volatile (__LOAD_ARGS_0                                       \
                    string : "=r" (err), "=r" (__o0) :                  \
                    "0" (err) :                                         \
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define inline_syscall1(string,err,name,arg1)				\
({									\
  register long __o0;                                                   \
  long __syscall_args[1];                                               \
  err = name;                                                           \
  __syscall_args[0] = (long) (arg1);					\
  __asm __volatile (__LOAD_ARGS_1                                       \
                    string : "=r" (err), "=r" (__o0) :                  \
                    "0" (err), "r" (__syscall_args) :                   \
                    __SYSCALL_CLOBBERS);                                \
__o0;                                                                   \
})

#define inline_syscall2(string,err,name,arg1,arg2)                      \
({									\
  register long __o0;                                                   \
  long __syscall_args[2];                                               \
  err = name;                                                           \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __asm __volatile (__LOAD_ARGS_2                                       \
                    string : "=r" (err), "=r" (__o0) :                  \
                    "0" (err), "r" (__syscall_args) :                   \
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define inline_syscall3(string,err,name,arg1,arg2,arg3)			\
({									\
  register long __o0;                                                   \
  long __syscall_args[3];                                               \
  err = name;                                                           \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __asm __volatile (__LOAD_ARGS_3                                       \
                    string : "=r" (err), "=r" (__o0) :                  \
                    "0" (err), "r" (__syscall_args) :                   \
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define inline_syscall4(string,err,name,arg1,arg2,arg3,arg4)		\
({									\
  register long __o0;                                                   \
  long __syscall_args[4];                                               \
  err = name;                                                           \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __syscall_args[3] = (long) (arg4);					\
  __asm __volatile (__LOAD_ARGS_4                                       \
                    string : "=r" (err), "=r" (__o0) :                  \
                    "0" (err), "r" (__syscall_args) :                   \
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define inline_syscall5(string,err,name,arg1,arg2,arg3,arg4,arg5)       \
({									\
  register long __o0;                                                   \
  long __syscall_args[5];                                               \
  err = name;                                                           \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __syscall_args[3] = (long) (arg4);					\
  __syscall_args[4] = (long) (arg5);					\
  __asm __volatile (__LOAD_ARGS_5                                       \
                    string : "=r" (err), "=r" (__o0) :                  \
                    "0" (err), "r" (__syscall_args) :                   \
                    __SYSCALL_CLOBBERS);				\
  __o0;                                                                 \
})

#define inline_syscall6(string,err,name,arg1,arg2,arg3,arg4,arg5,arg6)	\
({									\
  register long __o0;                                                   \
  long __syscall_args[6];                                               \
  err = name;                                                           \
  __syscall_args[0] = (long) (arg1);					\
  __syscall_args[1] = (long) (arg2);					\
  __syscall_args[2] = (long) (arg3);					\
  __syscall_args[3] = (long) (arg4);					\
  __syscall_args[4] = (long) (arg5);					\
  __syscall_args[5] = (long) (arg6);					\
  __asm __volatile (__LOAD_ARGS_6                                       \
                    string : "=r" (err), "=r" (__o0) :                  \
                    "0" (err), "r" (__syscall_args) :                   \
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
                    "=r" (__g1), "=r" (__o0), "=r" (__o1) :             \
                    "0" (__g1), "r" (__syscall_args) :                  \
                    __SYSCALL_CLOBBERS);				\
  if (INTERNAL_SYSCALL_ERROR_P (__o0, __g1))                            \
    {                                                                   \
      __set_errno (INTERNAL_SYSCALL_ERRNO (__o0, __g1));                \
      __o0 = -1L;                                                       \
    }                                                                   \
  else                                                                  \
    {                                                                   \
      __o0 &= (__o1 - 1);                                               \
    }                                                                   \
  __o0;                                                                 \
})

# else /* _LINUX_SPARC64_SYSDEP_H  */

/* Original definitions of `inline_syscall's are left
   intact here meanwhile. It's not clear if we have a
   problem related to Bug #50540, Comment #5 here at all.  */

#define inline_syscall0(string,err,name,dummy...)			\
({									\
	register long __o0 __asm__ ("o0");				\
	err = name;							\
	__asm __volatile (string : "=r" (err), "=r" (__o0) :		\
			  "0" (err) :					\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define inline_syscall1(string,err,name,arg1)				\
({									\
	register long __o0 __asm__ ("o0") = (long)(arg1);		\
	err = name;							\
	__asm __volatile (string : "=r" (err), "=r" (__o0) :		\
			  "0" (err), "1" (__o0) :			\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define inline_syscall2(string,err,name,arg1,arg2)			\
({									\
	register long __o0 __asm__ ("o0") = (long)(arg1);		\
	register long __o1 __asm__ ("o1") = (long)(arg2);		\
	err = name;							\
	__asm __volatile (string : "=r" (err), "=r" (__o0) :		\
			  "0" (err), "1" (__o0), "r" (__o1) :		\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define inline_syscall3(string,err,name,arg1,arg2,arg3)			\
({									\
	register long __o0 __asm__ ("o0") = (long)(arg1);		\
	register long __o1 __asm__ ("o1") = (long)(arg2);		\
	register long __o2 __asm__ ("o2") = (long)(arg3);		\
	err = name;							\
	__asm __volatile (string : "=r" (err), "=r" (__o0) :		\
			  "0" (err), "1" (__o0), "r" (__o1),		\
			  "r" (__o2) :					\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define inline_syscall4(string,err,name,arg1,arg2,arg3,arg4)		\
({									\
	register long __o0 __asm__ ("o0") = (long)(arg1);		\
	register long __o1 __asm__ ("o1") = (long)(arg2);		\
	register long __o2 __asm__ ("o2") = (long)(arg3);		\
	register long __o3 __asm__ ("o3") = (long)(arg4);		\
	err = name;							\
	__asm __volatile (string : "=r" (err), "=r" (__o0) :		\
			  "0" (err), "1" (__o0), "r" (__o1),		\
			  "r" (__o2), "r" (__o3) :			\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define inline_syscall5(string,err,name,arg1,arg2,arg3,arg4,arg5)	\
({									\
	register long __o0 __asm__ ("o0") = (long)(arg1);		\
	register long __o1 __asm__ ("o1") = (long)(arg2);		\
	register long __o2 __asm__ ("o2") = (long)(arg3);		\
	register long __o3 __asm__ ("o3") = (long)(arg4);		\
	register long __o4 __asm__ ("o4") = (long)(arg5);		\
	err = name;							\
	__asm __volatile (string : "=r" (err), "=r" (__o0) :		\
			  "0" (err), "1" (__o0), "r" (__o1),		\
			  "r" (__o2), "r" (__o3), "r" (__o4) :		\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define inline_syscall6(string,err,name,arg1,arg2,arg3,arg4,arg5,arg6)	\
({									\
	register long __o0 __asm__ ("o0") = (long)(arg1);		\
	register long __o1 __asm__ ("o1") = (long)(arg2);		\
	register long __o2 __asm__ ("o2") = (long)(arg3);		\
	register long __o3 __asm__ ("o3") = (long)(arg4);		\
	register long __o4 __asm__ ("o4") = (long)(arg5);		\
	register long __o5 __asm__ ("o5") = (long)(arg6);		\
	err = name;							\
	__asm __volatile (string : "=r" (err), "=r" (__o0) :		\
			  "0" (err), "1" (__o0), "r" (__o1),		\
			  "r" (__o2), "r" (__o3), "r" (__o4),		\
			  "r" (__o5) :					\
			  __SYSCALL_CLOBBERS);				\
	__o0;								\
})

#define INLINE_CLONE_SYSCALL(arg1,arg2,arg3,arg4,arg5)			\
({									\
	register long __o0 __asm__ ("o0") = (long)(arg1);		\
	register long __o1 __asm__ ("o1") = (long)(arg2);		\
	register long __o2 __asm__ ("o2") = (long)(arg3);		\
	register long __o3 __asm__ ("o3") = (long)(arg4);		\
	register long __o4 __asm__ ("o4") = (long)(arg5);		\
	register long __g1 __asm__ ("g1") = __NR_clone;			\
	__asm __volatile (__SYSCALL_STRING :				\
			  "=r" (__g1), "=r" (__o0), "=r" (__o1)	:	\
			  "0" (__g1), "1" (__o0), "2" (__o1),		\
			  "r" (__o2), "r" (__o3), "r" (__o4) :		\
			  __SYSCALL_CLOBBERS);				\
	if (INTERNAL_SYSCALL_ERROR_P (__o0, __g1))			\
	  {		     			       		   	\
	    __set_errno (INTERNAL_SYSCALL_ERRNO (__o0, __g1));		\
	    __o0 = -1L;			    				\
	  } 	      							\
	else								\
	  { 	      							\
	    __o0 &= (__o1 - 1);						\
	  } 	    	    						\
	__o0;								\
})

# endif /* _LINUX_SPARC64_SYSDEP_H  */

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
