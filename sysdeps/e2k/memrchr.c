/* Copyright (c) 2020 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: memrchr.c 5560 2020-03-01 08:29:24Z vlog $
 */

/* memrchr -- find the last occurrence of a byte in a memory block
   Copyright (C) 1991-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Based on strlen implementation by Torbjorn Granlund (tege@sics.se),
   with help from Dan Sahlin (dan@sics.se) and
   commentary by Jim Blandy (jimb@ai.mit.edu);
   adaptation to memchr suggested by Dick Karpinski (dick@cca.ucsf.edu),
   and implemented by Roland McGrath (roland@ai.mit.edu).

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

#include <stdlib.h>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#if defined _LIBC
# include <string.h>
# include <memcopy.h>
#endif

#include <sys/types.h>
#include <e2kintrin.h>

#undef __memrchr
#undef memrchr

#ifndef weak_alias
# define __memrchr memrchr
#endif

/* Search no more than N bytes of S for C.  */
void *
#ifndef MEMRCHR
__memrchr
#else
MEMRCHR
#endif
     (const void *s, int c_in, size_t n)
{
  const char *str = (const char *) s + n - 1;
  const char *end_ptr = (const char *) s;
  unsigned long long charmask;
  unsigned char c = (unsigned char) c_in;

  if (n == 0)
    return NULL;

  /* Set up a longword, each of whose bytes is C.  */
  charmask = __builtin_e2k_pshufb (c, c, 0);

#if __iset__ <= 4

  size_t tail;
  const unsigned long long *longword_ptr;
  unsigned long long longword, mask, align;

  align = E2K_BYTES_FROM_ALIGN (str, 8);
  longword_ptr = E2K_ALIGN_PTR_BACK (str, 8);

  longword = *longword_ptr;
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  mask &= (~0ULL) >> (56 - align * 8); /* обнуляем каждый байт после конца строки */

  if (mask != 0 || (const char *) longword_ptr < end_ptr) {
    /* Which of the bytes was the zero? */
    __CLZB (tail, mask);
    str = (const char *) longword_ptr + 7 - tail;
    return str >= end_ptr ? (void *) str : NULL;
  }

  /* откручиваем одну итерацию */
  longword = *--longword_ptr;
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  if (mask == 0 && (const char *) longword_ptr >= end_ptr) {

    /* We will test a 8 bytes at a time. */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
    for (;;) {
      longword = *--longword_ptr;
      mask = __builtin_e2k_pcmpeqb (longword, charmask);
      if (!(mask == 0 && (const char *) longword_ptr >= end_ptr)) break;
    }
  }
  /* Which of the bytes was the zero? */
  __CLZB (tail, mask);
  str = (const char *) longword_ptr + 7 - tail;


#elif __iset__ <= 5

#define __CMP(mask, x) /* bytes with 'c_in' -> bit mask */                    \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))

  const __v2di *qword_ptr;
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  __v2di qword;
  unsigned int mask, align;

  align = E2K_BYTES_FROM_ALIGN (str, 16);
  qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);

  qword = *qword_ptr;
  __CMP (mask, qword);
  mask &= 0xffff >> (15 - align); /* обнуляем каждый бит после конца строки */

  if (mask != 0 || (const char *) qword_ptr < end_ptr) {
    /* Which of the bytes was the zero? */
    str = (const char *) qword_ptr + 31 - __builtin_clz (mask);
    return str >= end_ptr ? (void *) str : NULL;
  }

  /* откручиваем одну итерацию */
  qword = *--qword_ptr;
  __CMP (mask, qword);
  if (mask == 0 && (const char *) qword_ptr >= end_ptr) {

    /* We will test a 16 bytes at a time. */
#pragma noprefetch
#pragma loop count (15) /* иначе применяется apb, 1 такт */
    for (;;) {
      qword = *--qword_ptr;
      __CMP (mask, qword);
      if (!(mask == 0 && (const char *) qword_ptr >= end_ptr)) break;
    }
  }
  /* Which of the bytes was the zero? */
  str = (const char *) qword_ptr + 31 - __builtin_clz (mask);


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

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) < 15 &&         /* closely to page border */
                        E2K_BYTES_FROM_ALIGN (str, 4096) >= n - 1, 0)) { /* string starts at the same page */
    /* Offsets 0-14 will be aligned into 0 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);
    __CMP (mask, *qword_ptr);
//    qword_ptr = E2K_ALIGN_PTR_BACK (str, 32);
//    FIND_CHAR (qword_ptr[0], qword_ptr[1]);
    mask <<= 31 - (str - (const char *) qword_ptr);
    /* Which of the bytes was C? */
    str -= __builtin_clz (mask);
    return str >= end_ptr ? (void *) str : NULL;
  }
  else {
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);
    /* first qword load is unaligned */
    qword = ((__v2di *) (str - 15))[0];

    if (__CMP_PRED (qword) || (str - 15) < end_ptr) {
      /* Which of the bytes was C? */
      __CMP (mask, qword);
      str += 16 - __builtin_clz (mask);
      return str >= end_ptr ? (void *) str : NULL;
    }
    /* next qword load is aligned */
    qword = *--qword_ptr;

    if (__CMP_PRED (qword) || (const char *) qword_ptr < end_ptr) {
      /* Which of the bytes was C? */
      __CMP (mask, qword);
      str = (const char *) qword_ptr + 31 - __builtin_clz (mask);
      return str >= end_ptr ? (void *) str : NULL;
    }
    qword_ptr = E2K_ALIGN_PTR_BACK (qword_ptr + 1, 32);
  }

  /* откручиваем одну итерацию */
  qword1 = *--qword_ptr;
  qword0 = *--qword_ptr;
  if (!(__CMP_PRED (qword0) || __CMP_PRED (qword1) || (const char *) qword_ptr < end_ptr)) {

    /* We will test a 32 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
    for (;;) {
      qword1 = *--qword_ptr;
      qword0 = *--qword_ptr;
      if (__CMP_PRED (qword0) || __CMP_PRED (qword1) || (const char *) qword_ptr < end_ptr) break;
    }
  }
  /* Which of the bytes was C? */
  FIND_CHAR (qword0, qword1);
  str = (const char *) qword_ptr + 31 - __builtin_clz (mask);

#endif /* __iset__ > 5 */

  return str >= end_ptr ? (void *) str : NULL;
}

#ifndef MEMRCHR
# ifdef weak_alias
weak_alias (__memrchr, memrchr)
# endif
#endif
