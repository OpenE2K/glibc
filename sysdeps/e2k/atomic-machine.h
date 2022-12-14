/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Atomic operations.  e2k32 version.
   Copyright (C) 2003, 2004, 2006 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

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

/* As far as I understand, unlike `sysdeps/sparc/sparc32/sparcv9/bits/atomic.h'
   we implement 64-bit atomics in 32-bit mode as well. Moreover, they say that
   they can do it if needed, but currently it's not the case.  */
#define __HAVE_64B_ATOMICS 1

/* FIXME: unfortunately generic NPTL code uses the above macro to deduce the
   alignment of arch-specific sem_t (for E2K the latter is defined in sysdeps/
   e2k/nptl/bits/semaphore.h). Whereas we really have 64-bit atomics in 32-bit
   mode, our sem_t has been stupidly borrowed from sparc from the very beginning
   and turns out to have 4-byte alignment in 32-bit mode, which led to Bug
   #80278 after a fix of semaphore destruction, which happened a year ago (see
   commit 042e1521c794a945edc43b5bfa7e69ad70420524). The underlying macro
   signalizes to generic NPTL code that it shouldn't assume sem_t to be aligned
   on 64-bit boundary despite __HAVE_64B_ATOMICS being set to 1.  */
#if __WORDSIZE == 32
# define __NEW_SEM_ALIGN_32 1
#endif

/* XXX Is this actually correct?  */
#define ATOMIC_EXCHANGE_USES_CAS 0


/* Use atomic builtins when configured appropriately.  */
#define USE_ATOMIC_COMPILER_BUILTINS 1

# define __arch_compare_and_exchange_bool_64_int(mem, newval, oldval, model) \
  ({									\
    typeof (*mem) __oldval = (oldval);					\
    !__atomic_compare_exchange_n (mem, (void *) &__oldval, newval, 0,	\
				  model, __ATOMIC_RELAXED);		\
  })

#define __arch_compare_and_exchange_bool_32_int(mem, newval, oldval, model) \
  __arch_compare_and_exchange_bool_64_int (mem, newval, oldval, model)

#define __arch_compare_and_exchange_bool_16_int(mem, newval, oldval, model) \
  (*((int *) 0) = 0, 0)

#define __arch_compare_and_exchange_bool_8_int(mem, newval, oldval, model) \
  (*((int *) 0) = 0, 0)


# define atomic_compare_and_exchange_bool_acq(mem, new, old)	\
  __atomic_bool_bysize (__arch_compare_and_exchange_bool, int,	\
			mem, new, old, __ATOMIC_ACQUIRE)



#define __arch_compare_and_exchange_val_64_int(mem, newval, oldval, model) \
  ({									\
    typeof (*mem) __oldval = (oldval);					\
    __atomic_compare_exchange_n (mem, (void *) &__oldval, newval, 0,	\
				 model, __ATOMIC_RELAXED);		\
    __oldval;								\
  })

#define __arch_compare_and_exchange_val_32_int(mem, newval, oldval, model) \
  __arch_compare_and_exchange_val_64_int (mem, newval, oldval, model)

#define __arch_compare_and_exchange_val_16_int(mem, newval, oldval, model) \
  (*((int *) 0) = 0, (__typeof (*mem)) 0)

#define __arch_compare_and_exchange_val_8_int(mem, newval, oldval, model) \
  (*((int *) 0) = 0, (__typeof (*mem)) 0)


# define atomic_compare_and_exchange_val_acq(mem, new, old)	\
  __atomic_val_bysize (__arch_compare_and_exchange_val, int,	\
		       mem, new, old, __ATOMIC_ACQUIRE)

# define atomic_compare_and_exchange_val_rel(mem, new, old)	\
  __atomic_val_bysize (__arch_compare_and_exchange_val, int,	\
		       mem, new, old, __ATOMIC_RELEASE)


# define __arch_exchange_64_int(mem, newval, model)	\
  __atomic_exchange_n (mem, newval, model)

# define __arch_exchange_32_int(mem,newval,model)	\
  __arch_exchange_64_int (mem, newval, model)

# define __arch_exchange_16_int(mem,newval,model)	\
  __arch_exchange_64_int (mem, newval, model)

# define __arch_exchange_8_int(mem,newval,model)	\
  __arch_exchange_64_int (mem, newval, model)


# define atomic_exchange_acq(mem, value)				\
  __atomic_val_bysize (__arch_exchange, int, mem, value, __ATOMIC_ACQUIRE)

# ifdef __ptr128__

int __ptr128_spinlock
__attribute__ ((nocommon, section (".gnu.linkonce.b.__ptr128_spinlock"
				   __sec_comment),
		visibility ("hidden")));

#  define atomic_exchange_ptr_acq(mem, value)				\
  ({									\
    extern int __ptr128_spinlock;					\
    void **__mem = (void **) mem;					\
    void *__oldval;							\
    while (atomic_compare_and_exchange_bool_acq (&__ptr128_spinlock, 1, 0)) \
      continue;								\
    									\
    __oldval =  *__mem;							\
      *__mem = value;							\
    									\
    atomic_exchange_acq (&__ptr128_spinlock, 0);			\
    __oldval;								\
  })

# define atomic_load_ptr_relaxed(mem)					\
  ({									\
    extern int __ptr128_spinlock;					\
    void **__mem;							\
    void *__oldval;							\
    if (sizeof (*mem) != 16)						\
      __atomic_link_error ();						\
									\
    __mem = (void **) mem;						\
    while (atomic_compare_and_exchange_bool_acq (&__ptr128_spinlock, 1, 0)) \
      continue;								\
									\
    __oldval =  *__mem;							\
									\
    atomic_exchange_acq (&__ptr128_spinlock, 0);			\
    __oldval;								\
									\
  })

/* FIXME: this is almost sure to be incorrect.  */
# define atomic_load_ptr_acquire(mem) atomic_load_ptr_relaxed (mem)

# define atomic_store_ptr_relaxed(mem, val)					\
  ({									\
    extern int __ptr128_spinlock;					\
    void **__mem;							\
    if (sizeof (*mem) != 16)						\
      __atomic_link_error ();						\
									\
    __mem = (void **) mem;						\
    while (atomic_compare_and_exchange_bool_acq (&__ptr128_spinlock, 1, 0)) \
      continue;								\
									\
    *__mem = val;							\
									\
    atomic_exchange_acq (&__ptr128_spinlock, 0);			\
  })

#  define atomic_compare_and_exchange_ptr_val_acq(mem, newval, oldval)	\
  ({									\
    extern int __ptr128_spinlock;					\
    void **__mem = (void **) mem;					\
    void *__oldval;							\
    while (atomic_compare_and_exchange_bool_acq (&__ptr128_spinlock, 1, 0)) \
      continue;								\
    									\
    __oldval =  *__mem;							\
    if (oldval == __oldval)						\
      *__mem = newval;							\
    									\
    atomic_exchange_acq (&__ptr128_spinlock, 0);			\
    __oldval;								\
  })


#  define atomic_compare_and_exchange_ptr_bool_acq(mem, newval, oldval) \
  ({									\
    int res = 1;							\
    extern int __ptr128_spinlock;					\
    void **__mem = (void **) mem;					\
    void *__oldval;							\
    while (atomic_compare_and_exchange_bool_acq (&__ptr128_spinlock, 1, 0)) \
      continue;								\
    									\
    __oldval =  *__mem;							\
    if (oldval == __oldval)						\
      {									\
	*__mem = newval;						\
	res = 0;							\
      }									\
    									\
    atomic_exchange_acq (&__ptr128_spinlock, 0);			\
    res;								\
  })

#  define catomic_compare_and_exchange_ptr_bool_acq(mem, newval, oldval) \
  atomic_compare_and_exchange_ptr_bool_acq (mem, newval, oldval)

#  define atomic_compare_exchange_ptr_weak_acquire(mem, expected, desired) \
  ({									\
    typeof (*(expected)) __atg102_expected = *(expected);		\
    *(expected) =							\
      atomic_compare_and_exchange_ptr_val_acq ((mem), (desired), *(expected)); \
    *(expected) == __atg102_expected;					\
  })

/* Defined by analogy with the default definition of `atomic_compare_and_
   exchange_val_rel ()' in `include/atomic.h'.  */
# define atomic_compare_and_exchange_ptr_val_rel(mem, newval, oldval)	\
  atomic_compare_and_exchange_ptr_val_acq (mem, newval, oldval)

/* Implemented by analogy with `atomic_compare_exchange_weak_release ()' in
   `include/atomic.h' in non-`USE_ATOMIC_COMPILER_BUILTINS' case.  */
#  define atomic_compare_exchange_ptr_weak_release(mem, expected, desired) \
  ({ typeof (*(expected)) __atg103_expected = *(expected);		\
    *(expected) =							\
      atomic_compare_and_exchange_ptr_val_rel ((mem), (desired), *(expected)); \
    *(expected) == __atg103_expected; })


#endif /* defined __ptr128__  */


# define atomic_exchange_rel(mem, value)				\
  __atomic_val_bysize (__arch_exchange, int, mem, value, __ATOMIC_RELEASE)


# define __arch_exchange_and_add_64_int(mem, value, model)	\
  __atomic_fetch_add (mem, value, model)

# define __arch_exchange_and_add_32_int(mem, value, model)	\
  __arch_exchange_and_add_64_int (mem, value, model)

# define __arch_exchange_and_add_16_int(mem, value, model)	\
  __arch_exchange_and_add_64_int (mem, value, model)

# define __arch_exchange_and_add_8_int(mem, value, model)	\
  __arch_exchange_and_add_64_int (mem, value, model)


# define atomic_exchange_and_add_acq(mem, value)			\
  __atomic_val_bysize (__arch_exchange_and_add, int, mem, value,	\
		       __ATOMIC_ACQUIRE)

# define atomic_exchange_and_add_rel(mem, value)			\
  __atomic_val_bysize (__arch_exchange_and_add, int, mem, value,	\
		       __ATOMIC_RELEASE)
