/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: memchr.c 2218 2014-06-16 13:57:59Z vlog $
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#undef __ptr_t
#define __ptr_t void *

#if defined _LIBC
# include <string.h>
# include <memcopy.h>
#endif

#if _LIBC || HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include <sys/types.h>
#include <e2kintrin.h>

#undef memchr
#undef __memchr

#ifndef MEMCHR
# define MEMCHR __memchr
#endif

/* Search no more than N bytes of S for C.  */
__ptr_t
MEMCHR (s, c_in, n)
     const __ptr_t s;
     int c_in;
     size_t n;
{
  const char *str = (const char *) s;
  const char *end_ptr = str + n;
  unsigned long long charmask;
  size_t tail;
  unsigned char c = (unsigned char) c_in;

  if (n == 0)
    return NULL;

  if (end_ptr < str)
    end_ptr = (const char *) ~0UL;

  /* Set up a longword, each of whose bytes is C.  */
  charmask = __builtin_e2k_pshufb (c, c, 0);

#if __iset__ <= 4

  const unsigned long long *longword_ptr;
  unsigned long long longword, mask, align;

  align = E2K_BYTES_FROM_ALIGN (str, 8);
  longword_ptr = E2K_ALIGN_PTR_BACK (str, 8);

  longword = *longword_ptr++;
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  mask &= (-1LL) << (align * 8); /* обнуляем каждый байт до начала строки */

  if (mask != 0 || (const char *) longword_ptr >= end_ptr) {
    /* Which of the bytes was the zero? */
    __CTZB (tail, mask);
    str = (const char *) (longword_ptr - 1) + tail;
    return str >= end_ptr ? NULL : (__ptr_t) str;
  }

  /* откручиваем одну итерацию */
  longword = *longword_ptr++;
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  if (mask == 0 && (const char *) longword_ptr < end_ptr) {

    /* We will test a 8 bytes at a time. */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
    for (;;) {
      longword = *longword_ptr++;
      mask = __builtin_e2k_pcmpeqb (longword, charmask);
      if (!(mask == 0 && (const char *) longword_ptr < end_ptr)) break;
    }
  }
  /* Which of the bytes was the zero? */
  __CTZB (tail, mask);
  str = (const char *) (longword_ptr - 1) + tail;


#elif __iset__ <= 5

#define __CMP(mask, x) /* bytes with 'c_in' -> bit mask */                    \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))

  const __v2di *qword_ptr;
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  __v2di qword;
  unsigned int mask, align;

  align = E2K_BYTES_FROM_ALIGN (str, 16);
  qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);

  qword = *qword_ptr++;
  __CMP (mask, qword);
  mask &= (-1) << align; /* обнуляем каждый бит до начала строки */

  if (mask != 0 || (const char *) qword_ptr >= end_ptr) {
    /* Which of the bytes was the zero? */
    tail = __builtin_ctz (mask);
    str = (const char *) (qword_ptr - 1) + tail;
    return str >= end_ptr ? NULL : (__ptr_t) str;
  }

  /* откручиваем одну итерацию */
  qword = *qword_ptr++;
  __CMP (mask, qword);
  if (mask == 0 && (const char *) qword_ptr < end_ptr) {

    /* We will test a 16 bytes at a time. */
#pragma noprefetch
#pragma loop count (15) /* иначе применяется apb, 1 такт */
    for (;;) {
      qword = *qword_ptr++;
      __CMP (mask, qword);
      if (!(mask == 0 && (const char *) qword_ptr < end_ptr)) break;
    }
  }
  /* Which of the bytes was the zero? */
  tail = __builtin_ctz (mask);
  str = (const char *) (qword_ptr - 1) + tail;


#else /* __iset__ > 5 */

#define __CMP(mask, x) /* bytes with 'c_in' -> bit mask */                    \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))
#define __CMP_PRED(x)  /* bytes with 'c_in' -> predicate */                   \
    __builtin_e2k_qpcmpeqbop (x, qcharmask)

#define FIND_CHAR(qword0, qword1) {                       \
  unsigned int mask0, mask1;                              \
  __CMP (mask0, qword0);                                  \
  __CMP (mask1, qword1);                                  \
  mask = (mask1 << 16) | mask0;                           \
}

  const __v2di *qword_ptr;
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  __v2di qword, qword0, qword1;
  unsigned int mask;

  qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4080 &&          /* closely to page border */
      E2K_ALIGN_PTR_BACK (str, 4096) == E2K_ALIGN_PTR_BACK (end_ptr - 1, 4096), 0)) { /* string ends at the same page */
    /* Offsets 4081-4095 will be aligned into 4080 thus fit into page */
    __CMP (mask, *qword_ptr);
    mask >>= (str - (const char *) qword_ptr);
    /* Which of the bytes was C? */
    str += __builtin_ctz (mask);
    return str >= end_ptr ? NULL : (__ptr_t) str;
  }
  else {
    /* first qword load is unaligned */
    qword = ((__v2di *) str)[0];

    if (__CMP_PRED (qword) || (str + 16) >= end_ptr) {
      /* Which of the bytes was C? */
      __CMP (mask, qword);
      str += __builtin_ctz (mask);
      return str >= end_ptr ? NULL : (__ptr_t) str;
    }
    /* next qword load is aligned */
    qword = qword_ptr[1];

    if (__CMP_PRED (qword) || (const char *) (qword_ptr + 2) >= end_ptr) {
      /* Which of the bytes was C? */
      __CMP (mask, qword);
      tail = __builtin_ctz (mask);
      str = (const char *) (qword_ptr + 1) + tail;
      return str >= end_ptr ? NULL : (__ptr_t) str;
    }
    qword_ptr = E2K_ALIGN_PTR_BACK (qword_ptr + 2, 32);
  }

  /* откручиваем одну итерацию */
  qword0 = qword_ptr[0];
  qword1 = qword_ptr[1];
  qword_ptr += 2;
  if (!(__CMP_PRED (qword0) || __CMP_PRED (qword1) || (const char *) qword_ptr >= end_ptr)) {

    /* We will test a 32 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
    for (;;) {
      qword0 = qword_ptr[0];
      qword1 = qword_ptr[1];
      qword_ptr += 2;
      if (__CMP_PRED (qword0) || __CMP_PRED (qword1) || (const char *) qword_ptr >= end_ptr) break;
    }
  }
  /* Which of the bytes was C? */
  FIND_CHAR (qword0, qword1);
  tail = __builtin_ctz (mask);
  str = (const char *) (qword_ptr - 2) + tail;

#endif /* __iset__ > 5 */

  return str >= end_ptr ? NULL : (__ptr_t) str;
}

#ifdef weak_alias
weak_alias (__memchr, memchr)
#endif
libc_hidden_builtin_def (memchr)
