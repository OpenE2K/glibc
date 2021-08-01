/* Atomic operations.  sparc32 version.
   Copyright (C) 2003-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 2003.

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

#ifndef _ATOMIC_MACHINE_H
#define _ATOMIC_MACHINE_H	1

#include <stdint.h>

typedef int8_t atomic8_t;
typedef uint8_t uatomic8_t;
typedef int_fast8_t atomic_fast8_t;
typedef uint_fast8_t uatomic_fast8_t;

typedef int16_t atomic16_t;
typedef uint16_t uatomic16_t;
typedef int_fast16_t atomic_fast16_t;
typedef uint_fast16_t uatomic_fast16_t;

typedef int32_t atomic32_t;
typedef uint32_t uatomic32_t;
typedef int_fast32_t atomic_fast32_t;
typedef uint_fast32_t uatomic_fast32_t;

typedef int64_t atomic64_t;
typedef uint64_t uatomic64_t;
typedef int_fast64_t atomic_fast64_t;
typedef uint_fast64_t uatomic_fast64_t;

typedef intptr_t atomicptr_t;
typedef uintptr_t uatomicptr_t;
typedef intmax_t atomic_max_t;
typedef uintmax_t uatomic_max_t;

#ifndef __LCC__
#define CHECK_SIZE(mem)                         \
  if (sizeof (*mem) != 4)                       \
    abort ()
#else
#define CHECK_SIZE(mem)                         \
  if (sizeof (*mem) != 4)                       \
    *((int *) 0) = 0
#endif

#define __HAVE_64B_ATOMICS 0
#define USE_ATOMIC_COMPILER_BUILTINS 0

/* XXX Is this actually correct?  */
#define ATOMIC_EXCHANGE_USES_CAS 1


/* We have no compare and swap, just test and set.
   The following implementation contends on 64 global locks
   per library and assumes no variable will be accessed using atomic.h
   macros from two different libraries.  */

__make_section_unallocated
  (".gnu.linkonce.b.__sparc32_atomic_locks, \"aw\", %nobits");

volatile unsigned char __sparc32_atomic_locks[64]
  __attribute__ ((nocommon, section (".gnu.linkonce.b.__sparc32_atomic_locks"
				     __sec_comment),
		  visibility ("hidden")));

#define __sparc32_atomic_do_lock(addr) \
  do								      \
    {								      \
      unsigned int __old_lock;					      \
      unsigned int __idx = (((long) addr >> 2) ^ ((long) addr >> 12)) \
			   & 63;				      \
      do							      \
	__asm __volatile ("ldstub %1, %0"			      \
			  : "=r" (__old_lock),			      \
			    "=m" (__sparc32_atomic_locks[__idx])      \
			  : "m" (__sparc32_atomic_locks[__idx])	      \
			  : "memory");				      \
      while (__old_lock);					      \
    }								      \
  while (0)

#define __sparc32_atomic_do_unlock(addr) \
  do								      \
    {								      \
      __sparc32_atomic_locks[(((long) addr >> 2)		      \
			      ^ ((long) addr >> 12)) & 63] = 0;	      \
      __asm __volatile ("" ::: "memory");			      \
    }								      \
  while (0)

#define __sparc32_atomic_do_lock24(addr) \
  do								      \
    {								      \
      unsigned int __old_lock;					      \
      do							      \
	__asm __volatile ("ldstub %1, %0"			      \
			  : "=r" (__old_lock), "=m" (*(addr))	      \
			  : "m" (*(addr))			      \
			  : "memory");				      \
      while (__old_lock);					      \
    }								      \
  while (0)

#define __sparc32_atomic_do_unlock24(addr) \
  do								      \
    {								      \
      __asm __volatile ("" ::: "memory");			      \
      *(char *) (addr) = 0;					      \
    }								      \
  while (0)


#ifndef SHARED
# define __v9_compare_and_exchange_val_32(mem, newval, oldval) \
({union { __typeof (oldval) a; uint32_t v; } oldval_arg = { .a = (oldval) };  \
  union { __typeof (newval) a; uint32_t v; } newval_arg = { .a = (newval) };  \
  register uint32_t __acev_tmp __asm ("%g6");			              \
  register __typeof (mem) __acev_mem __asm ("%g1") = (mem);		      \
  register uint32_t __acev_oldval __asm ("%g5");		              \
  __acev_tmp = newval_arg.v;						      \
  __acev_oldval = oldval_arg.v;						      \
  /* .word 0xcde05005 is cas [%g1], %g5, %g6.  Can't use cas here though,     \
     because as will then mark the object file as V8+ arch.  */		      \
  __asm __volatile (".word 0xcde05005"					      \
		    : "+r" (__acev_tmp), "=m" (*__acev_mem)		      \
		    : "r" (__acev_oldval), "m" (*__acev_mem),		      \
		      "r" (__acev_mem) : "memory");			      \
  (__typeof (oldval)) __acev_tmp; })

#define __v9_compare_and_exchange_val_32_acq(mem, newval, oldval) \
({ \
  __typeof (*(mem)) __a; \
  __a = __v9_compare_and_exchange_val_32(mem, newval, oldval); \
  atomic_read_barrier(); \
  __a; \
})

#define __v9_compare_and_exchange_val_32_rel(mem, newval, oldval) \
({ \
  atomic_write_barrier();\
  __v9_compare_and_exchange_val_32(mem, newval, oldval); \
})

#endif

#ifdef SUPPORT_SYS_ATOMIC
#define __e90_compare_and_exchange_val_32_acq(mem, newval, oldval)      \
({                                                                      \
  __typeof (*(mem)) __acev_old;                                         \
  __typeof (mem) __acev_mem = (mem);                                    \
  __asm __volatile ("mov 0x1f3, %%g1;"                                  \
                    "mov 0x1, %%o0;"                                    \
                    "mov 0x4, %%o1;"                                    \
                    "mov %2, %%o2;"                                     \
                    "mov %3, %%o3;"                                     \
                    "mov %4, %%o4;"                                     \
                    "mov %5, %%o5;"                                     \
                    "ta 0x10"                                           \
                    : "=m" (__acev_old), "=m" (*__acev_mem)             \
                    : "r" (__acev_mem), "r" (oldval),                   \
                    "r" (newval), "r" (&__acev_old),                    \
                    "m" (__acev_old), "m" (*__acev_mem)                 \
                    : "o0", "o1", "o2", "o3", "o4", "o5",               \
                    "g1", "g2", "g3", "g4", "g5", "g6",                 \
                    "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",     \
                    "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", \
                    "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", \
                    "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31", \
                    "cc", "memory");                                    \
  __acev_old; })
#endif

/* The only basic operation needed is compare and exchange.  */
#define __v7_compare_and_exchange_val_acq(mem, newval, oldval) \
  ({ __typeof (mem) __acev_memp = (mem);			      \
     __typeof (*mem) __acev_ret;				      \
     __typeof (*mem) __acev_newval = (newval);			      \
								      \
     __sparc32_atomic_do_lock (__acev_memp);			      \
     __acev_ret = *__acev_memp;					      \
     if (__acev_ret == (oldval))				      \
       *__acev_memp = __acev_newval;				      \
     __sparc32_atomic_do_unlock (__acev_memp);			      \
     __acev_ret; })

#define __v7_compare_and_exchange_bool_acq(mem, newval, oldval) \
  ({ __typeof (mem) __aceb_memp = (mem);			      \
     int __aceb_ret;						      \
     __typeof (*mem) __aceb_newval = (newval);			      \
								      \
     __sparc32_atomic_do_lock (__aceb_memp);			      \
     __aceb_ret = 0;						      \
     if (*__aceb_memp == (oldval))				      \
       *__aceb_memp = __aceb_newval;				      \
     else							      \
       __aceb_ret = 1;						      \
     __sparc32_atomic_do_unlock (__aceb_memp);			      \
     __aceb_ret; })

#define __v7_exchange_acq(mem, newval) \
  ({ __typeof (mem) __acev_memp = (mem);			      \
     __typeof (*mem) __acev_ret;				      \
     __typeof (*mem) __acev_newval = (newval);			      \
								      \
     __sparc32_atomic_do_lock (__acev_memp);			      \
     __acev_ret = *__acev_memp;					      \
     *__acev_memp = __acev_newval;				      \
     __sparc32_atomic_do_unlock (__acev_memp);			      \
     __acev_ret; })

#define __v7_exchange_and_add(mem, value) \
  ({ __typeof (mem) __acev_memp = (mem);			      \
     __typeof (*mem) __acev_ret;				      \
								      \
     __sparc32_atomic_do_lock (__acev_memp);			      \
     __acev_ret = *__acev_memp;					      \
     *__acev_memp = __acev_ret + (value);			      \
     __sparc32_atomic_do_unlock (__acev_memp);			      \
     __acev_ret; })

/* Special versions, which guarantee that top 8 bits of all values
   are cleared and use those bits as the ldstub lock.  */
#define __v7_compare_and_exchange_val_24_acq(mem, newval, oldval) \
  ({ __typeof (mem) __acev_memp = (mem);			      \
     __typeof (*mem) __acev_ret;				      \
     __typeof (*mem) __acev_newval = (newval);			      \
								      \
     __sparc32_atomic_do_lock24 (__acev_memp);			      \
     __acev_ret = *__acev_memp & 0xffffff;			      \
     if (__acev_ret == (oldval))				      \
       *__acev_memp = __acev_newval;				      \
     else							      \
       __sparc32_atomic_do_unlock24 (__acev_memp);		      \
     __asm __volatile ("" ::: "memory");			      \
     __acev_ret; })

#define __v7_exchange_24_rel(mem, newval) \
  ({ __typeof (mem) __acev_memp = (mem);			      \
     __typeof (*mem) __acev_ret;				      \
     __typeof (*mem) __acev_newval = (newval);			      \
								      \
     __sparc32_atomic_do_lock24 (__acev_memp);			      \
     __acev_ret = *__acev_memp & 0xffffff;			      \
     *__acev_memp = __acev_newval;				      \
     __asm __volatile ("" ::: "memory");			      \
     __acev_ret; })



#ifdef SUPPORT_SYS_ATOMIC
/* Kernel atomic ops may be either present or not at V8 and we
   should be able to support them both in static and dynamic
   libraries (see Bug #54479 at the end of Comment #15). Therefore
   `__atomic_is_e90'  is defined both for if(n)def SHARED below
   though in a different way.  */
# define __ATOMIC_HWCAP_SPARC_SYS_ATOMIC        0x10000000
#endif /* SUPPORT_SYS_ATOMIC  */


#ifdef SHARED

/* When dynamically linked, we assume pre-v9 libraries are only ever
   used on pre-v9 CPU.  */
# define __atomic_is_v9 0


# ifndef SUPPORT_SYS_ATOMIC

#  define atomic_compare_and_exchange_val_acq(mem, newval, oldval) \
  __v7_compare_and_exchange_val_acq (mem, newval, oldval)

#  define atomic_compare_and_exchange_bool_acq(mem, newval, oldval) \
  __v7_compare_and_exchange_bool_acq (mem, newval, oldval)

#  define atomic_exchange_acq(mem, newval) \
  __v7_exchange_acq (mem, newval)

#  define atomic_exchange_and_add(mem, value) \
  __v7_exchange_and_add (mem, value)

#  define atomic_compare_and_exchange_val_24_acq(mem, newval, oldval)  \
  ({                                                                   \
    CHECK_SIZE (mem);                                                  \
    __v7_compare_and_exchange_val_24_acq (mem, newval, oldval); })

#  define atomic_exchange_24_rel(mem, newval) \
  ({								      \
    CHECK_SIZE (mem);                                                 \
    __v7_exchange_24_rel (mem, newval); })

# else /* SUPPORT_SYS_ATOMIC  */

/* They don't want to include <ldsodefs.h> here. But I really need
   to know where to find `_rtld_{global,local}_ro._dl_hwcap' in
   ld.so . . . If I am not mistaken, it has offset 64 in bytes from
   the start of the structure.  */
#  if !IS_IN (rtld)
extern uint64_t _rtld_global_ro_arr[]
__attribute__((section(".data.rel.ro"))) __asm__ ("_rtld_global_ro");

/* Hosts with the atomic ops implemented inside the Kernel are rare
   (see Bug #54479). Libraries configured with `--with-sys-atomic'
   may very well be installed to a host where this feature is not
   enabled. Therefore specify `0' in the second `__builtin_expect ()'.  */
# define __atomic_is_e90 \
  (__builtin_expect (_rtld_global_ro_arr[8] & __ATOMIC_HWCAP_SPARC_SYS_ATOMIC, \
                     0))
#  else
extern uint64_t _rtld_local_ro_arr[]
__attribute__ ((section (".data.rel.ro")))
__attribute__ ((visibility ("hidden"))) __asm__ ("_rtld_local_ro");

# define __atomic_is_e90 \
  (__builtin_expect (_rtld_local_ro_arr[8] & __ATOMIC_HWCAP_SPARC_SYS_ATOMIC,  \
                     0))
#  endif



#  define atomic_compare_and_exchange_val_acq(mem, newval, oldval)      \
  ({                                                                    \
    __typeof (*mem) __acev_wret;                                        \
    CHECK_SIZE (mem);                                                   \
    if (__atomic_is_e90)                                                \
      __acev_wret                                                       \
        = __e90_compare_and_exchange_val_32_acq (mem, newval,           \
                                                 oldval);               \
    else                                                                \
      __acev_wret                                                       \
        = __v7_compare_and_exchange_val_acq (mem, newval,               \
                                             oldval);                   \
    __acev_wret; })

#  define atomic_compare_and_exchange_bool_acq(mem, newval, oldval) \
  ({                                                                  \
  int __acev_wret;						      \
  CHECK_SIZE (mem);                                                   \
  if (__atomic_is_e90)                                                \
    {                                                                 \
      __typeof (oldval) __acev_woldval = (oldval);		      \
      __acev_wret						      \
        = __e90_compare_and_exchange_val_32_acq (mem, newval,         \
                                                 __acev_woldval)      \
        != __acev_woldval;					      \
    }                                                                 \
  else                                                                \
    __acev_wret                                                       \
      =  __v7_compare_and_exchange_bool_acq (mem, newval, oldval);    \
  __acev_wret; })

#  define atomic_exchange_acq(mem, newval) \
  ({								      \
    __typeof (*mem) __acev_wret;				      \
    CHECK_SIZE (mem);                                                 \
    if (__atomic_is_e90)					      \
      {                                                               \
        __typeof (mem) __acev_wmemp = (mem);			      \
        __typeof (*(mem)) __acev_wval = (newval);		      \
        do							      \
          __acev_wret = *__acev_wmemp;				      \
        while (__builtin_expect                                       \
               (__e90_compare_and_exchange_val_32_acq (__acev_wmemp,  \
                                                       __acev_wval,   \
                                                       __acev_wret)   \
                != __acev_wret, 0));				      \
       }							      \
     else							      \
       __acev_wret = __v7_exchange_acq (mem, newval);                 \
     __acev_wret; })

#  define atomic_exchange_and_add(mem, value) \
  ({                                                                    \
    __typeof (*mem) __acev_wret;                                        \
    CHECK_SIZE (mem);                                                   \
    if (__atomic_is_e90)                                                \
      {                                                                 \
        __typeof (mem) __acev_wmemp = (mem);                            \
        __typeof (*(mem)) __acev_wval = (value);                        \
        do                                                              \
          __acev_wret = *__acev_wmemp;                                  \
        while (__builtin_expect                                         \
               (__e90_compare_and_exchange_val_32_acq (__acev_wmemp,    \
                                                       (__acev_wret     \
                                                        + __acev_wval), \
                                                       __acev_wret)     \
                != __acev_wret, 0));                                    \
      }                                                                 \
    else                                                                \
      __acev_wret = __v7_exchange_and_add (mem, value);                 \
    __acev_wret; })


#  define atomic_compare_and_exchange_val_24_acq(mem, newval, oldval) \
  ({                                                                    \
    __typeof (*mem) __acev_wret;                                        \
    CHECK_SIZE (mem);                                                   \
    if (__atomic_is_e90)                                                \
      __acev_wret                                                       \
        = __e90_compare_and_exchange_val_32_acq (mem, newval,           \
                                                 oldval);               \
    else                                                                \
      __acev_wret                                                       \
        = __v7_compare_and_exchange_val_24_acq (mem, newval, oldval);   \
    __acev_wret; })

#  define atomic_exchange_24_rel(mem, newval)                           \
  ({                                                                    \
    __typeof (*mem) __acev_w24ret;                                      \
    CHECK_SIZE (mem);                                                   \
    if (__atomic_is_e90)                                                \
      __acev_w24ret = atomic_exchange_rel (mem, newval);                \
    else                                                                \
      __acev_w24ret = __v7_exchange_24_rel (mem, newval);               \
    __acev_w24ret; })

# endif /* SUPPORT_SYS_ATOMIC  */

/* Do we need anything special for e90 with sys_atomic support here?  */
# define atomic_full_barrier() __asm ("" ::: "memory")
# define atomic_read_barrier() atomic_full_barrier ()
# define atomic_write_barrier() atomic_full_barrier ()


#else /* SHARED  */

/* In libc.a/libpthread.a etc. we don't know if we'll be run on
   pre-v9 or v9 CPU.  To be interoperable with dynamically linked
   apps on v9 CPUs e.g. with process shared primitives, use cas insn
   on v9 CPUs and ldstub on pre-v9.  */

extern uint64_t _dl_hwcap __attribute__((weak));
# define __atomic_is_v9 \
  (__builtin_expect (&_dl_hwcap != 0, 1) \
   && __builtin_expect (_dl_hwcap & HWCAP_SPARC_V9, HWCAP_SPARC_V9))

# ifndef SUPPORT_SYS_ATOMIC

#  define atomic_compare_and_exchange_val_acq(mem, newval, oldval)       \
  ({                                                                    \
    __typeof (*mem) __acev_wret;                                        \
    CHECK_SIZE (mem);                                                   \
    if (__atomic_is_v9)                                                 \
      __acev_wret                                                       \
        = __v9_compare_and_exchange_val_32_acq (mem, newval, oldval);   \
    else                                                                \
      __acev_wret                                                       \
        = __v7_compare_and_exchange_val_acq (mem, newval, oldval);      \
    __acev_wret; })

#  define atomic_compare_and_exchange_bool_acq(mem, newval, oldval) \
  ({								      \
     int __acev_wret;						      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9)					      \
       {							      \
	 __typeof (oldval) __acev_woldval = (oldval);		      \
	 __acev_wret						      \
	   = __v9_compare_and_exchange_val_32_acq (mem, newval,	      \
						   __acev_woldval)    \
	     != __acev_woldval;					      \
       }							      \
     else							      \
       __acev_wret						      \
	 = __v7_compare_and_exchange_bool_acq (mem, newval, oldval);  \
     __acev_wret; })

#  define atomic_exchange_rel(mem, newval) \
  ({								      \
     __typeof (*mem) __acev_wret;				      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9)					      \
       {							      \
	 __typeof (mem) __acev_wmemp = (mem);			      \
	 __typeof (*(mem)) __acev_wval = (newval);		      \
	 do							      \
	   __acev_wret = *__acev_wmemp;				      \
	 while (__builtin_expect				      \
		  (__v9_compare_and_exchange_val_32_rel (__acev_wmemp,\
							 __acev_wval, \
							 __acev_wret) \
		   != __acev_wret, 0));				      \
       }							      \
     else							      \
       __acev_wret = __v7_exchange_acq (mem, newval);		      \
     __acev_wret; })

#  define atomic_compare_and_exchange_val_24_acq(mem, newval, oldval) \
  ({								      \
     __typeof (*mem) __acev_wret;				      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9)					      \
       __acev_wret						      \
	 = __v9_compare_and_exchange_val_32_acq (mem, newval, oldval);\
     else							      \
       __acev_wret						      \
	 = __v7_compare_and_exchange_val_24_acq (mem, newval, oldval);\
     __acev_wret; })

#  define atomic_exchange_24_rel(mem, newval) \
  ({								      \
     __typeof (*mem) __acev_w24ret;				      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9)                                              \
       __acev_w24ret = atomic_exchange_rel (mem, newval);	      \
     else							      \
       __acev_w24ret = __v7_exchange_24_rel (mem, newval);	      \
     __acev_w24ret; })


# else /* SUPPORT_SYS_ATOMIC  */

# define __atomic_is_e90 \
  (__builtin_expect (&_dl_hwcap != 0, 1) \
   && __builtin_expect (_dl_hwcap & __ATOMIC_HWCAP_SPARC_SYS_ATOMIC, \
                        0))


#  define atomic_compare_and_exchange_val_acq(mem, newval, oldval)       \
  ({                                                                    \
    __typeof (*mem) __acev_wret;                                        \
    CHECK_SIZE (mem);                                                   \
    if (__atomic_is_v9)                                                 \
      __acev_wret                                                       \
        = __v9_compare_and_exchange_val_32_acq (mem, newval, oldval);   \
    else if (__atomic_is_e90)                                           \
      __acev_wret                                                       \
        = __e90_compare_and_exchange_val_32_acq (mem, newval, oldval);  \
    else                                                                \
      __acev_wret                                                       \
        = __v7_compare_and_exchange_val_acq (mem, newval, oldval);      \
    __acev_wret; })

#  define atomic_compare_and_exchange_bool_acq(mem, newval, oldval) \
  ({								      \
     int __acev_wret;						      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9)					      \
       {							      \
	 __typeof (oldval) __acev_woldval = (oldval);		      \
	 __acev_wret						      \
	   = __v9_compare_and_exchange_val_32_acq (mem, newval,	      \
						   __acev_woldval)    \
	     != __acev_woldval;					      \
       }							      \
     else if (__atomic_is_e90)					      \
       {							      \
	 __typeof (oldval) __acev_woldval = (oldval);		      \
	 __acev_wret						      \
	   = __e90_compare_and_exchange_val_32_acq (mem, newval,      \
                                                    __acev_woldval)   \
	     != __acev_woldval;					      \
       }							      \
     else							      \
       __acev_wret						      \
	 = __v7_compare_and_exchange_bool_acq (mem, newval, oldval);  \
     __acev_wret; })

#  define atomic_exchange_rel(mem, newval) \
  ({								      \
     __typeof (*mem) __acev_wret;				      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9)					      \
       {							      \
	 __typeof (mem) __acev_wmemp = (mem);			      \
	 __typeof (*(mem)) __acev_wval = (newval);		      \
	 do							      \
	   __acev_wret = *__acev_wmemp;				      \
	 while (__builtin_expect				      \
		  (__v9_compare_and_exchange_val_32_rel (__acev_wmemp,\
							 __acev_wval, \
							 __acev_wret) \
		   != __acev_wret, 0));				      \
       }							      \
     else if (__atomic_is_e90)					      \
       {							      \
	 __typeof (mem) __acev_wmemp = (mem);			      \
	 __typeof (*(mem)) __acev_wval = (newval);		      \
	 do							      \
	   __acev_wret = *__acev_wmemp;				      \
	 while (__builtin_expect				      \
                (__e90_compare_and_exchange_val_32_acq (__acev_wmemp, \
                                                        __acev_wval,  \
                                                        __acev_wret)  \
                 != __acev_wret, 0));				      \
       }							      \
     else							      \
       __acev_wret = __v7_exchange_acq (mem, newval);		      \
     __acev_wret; })

#  define atomic_compare_and_exchange_val_24_acq(mem, newval, oldval) \
  ({								      \
     __typeof (*mem) __acev_wret;				      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9)					      \
       __acev_wret						      \
	 = __v9_compare_and_exchange_val_32_acq (mem, newval, oldval);\
     else if (__atomic_is_e90)                                        \
       __acev_wret                                                    \
         = __e90_compare_and_exchange_val_32_acq (mem, newval,        \
                                                  oldval);            \
     else							      \
       __acev_wret						      \
	 = __v7_compare_and_exchange_val_24_acq (mem, newval, oldval);\
     __acev_wret; })

#  define atomic_exchange_24_rel(mem, newval) \
  ({								      \
     __typeof (*mem) __acev_w24ret;				      \
     CHECK_SIZE (mem);                                                \
     if (__atomic_is_v9 || __atomic_is_e90)                           \
       __acev_w24ret = atomic_exchange_rel (mem, newval);	      \
     else							      \
       __acev_w24ret = __v7_exchange_24_rel (mem, newval);	      \
     __acev_w24ret; })

# endif /* SUPPORT_SYS_ATOMIC  */

/* Do we need anything special for e90 with sys_atomic support here?  */
#define atomic_full_barrier()						\
  do {									\
     if (__atomic_is_v9)						\
       /* membar #LoadLoad | #LoadStore | #StoreLoad | #StoreStore */	\
       __asm __volatile (".word 0x8143e00f" : : : "memory");		\
     else								\
       __asm __volatile ("" : : : "memory");				\
  } while (0)

#define atomic_read_barrier()						\
  do {									\
     if (__atomic_is_v9)						\
       /* membar #LoadLoad | #LoadStore */				\
       __asm __volatile (".word 0x8143e005" : : : "memory");		\
     else								\
       __asm __volatile ("" : : : "memory");				\
  } while (0)

#define atomic_write_barrier()						\
  do {									\
     if (__atomic_is_v9)						\
       /* membar  #LoadStore | #StoreStore */				\
       __asm __volatile (".word 0x8143e00c" : : : "memory");		\
     else								\
       __asm __volatile ("" : : : "memory");				\
  } while (0)

#endif /* SHARED  */

#include <sysdep.h>

#endif	/* atomic-machine.h */
