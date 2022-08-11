/* Copyright (c) 2015-2021 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strncat.c 2218 2014-06-16 13:57:59Z vlog $
 */

/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#include <string.h>

#ifdef _LIBC
# include <memcopy.h>
#endif

#include <e2kintrin.h>

#ifndef STRNCAT
# undef strncat
# define STRNCAT  strncat
#endif

#define restrict __restrict__

char *
STRNCAT (char * restrict s1, const char * restrict s2, size_t n)
{
  unsigned char * restrict srcp = (unsigned char *) s2;
  unsigned char * restrict dstp = (unsigned char *) s1;
  unsigned char * restrict end_ptr;
  long int tail;

#if __iset__ <= 4

#define FIND_ZERO(longword0, longword1) {                 \
  unsigned long long mask0, mask1;                        \
  mask0 = __builtin_e2k_pcmpeqb (longword0, 0);           \
  mask1 = __builtin_e2k_pcmpeqb (longword1, 0);           \
  mask = __builtin_e2k_pmovmskb (mask1, mask0);           \
}

  op_t aligns, alignd;
  size_t align;
  op_t a0, a00, a01, spec, mask;
  const unsigned long long *longword_ptr;
  unsigned long long longword, longword0, longword1;
  unsigned char c;

  /* Find the end of the string.  */
  /*  dstp += strlen (s1); */
  /* почти strlen в чистом виде */

  align = (size_t) s1 & 7;
  longword_ptr = E2K_ALIGN_PTR_BACK (s1, 8);

  /* first longword */
  longword = longword_ptr[0];
  mask = __builtin_e2k_pcmpeqb (longword, 0) >> (8 * align);

  if (mask != 0) {
    /* Which of the bytes was the zero? */
    __CTZB (tail, mask);
    dstp += tail;
    goto start_strncpy;
  }

  /* second longword */
  longword = longword_ptr[1];
  mask = __builtin_e2k_pcmpeqb (longword, 0);

  if (mask != 0) {
    /* Which of the bytes was the zero? */
    __CTZB (tail, mask);
    dstp += (const char *) (longword_ptr + 1) - s1 + tail;
    goto start_strncpy;
  }
  longword_ptr = E2K_ALIGN_PTR_BACK (longword_ptr + 2, 16);

  /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (8)
  for (;;) {
    longword0 = longword_ptr[0];
    longword1 = longword_ptr[1];
    longword = __builtin_e2k_pminub (longword0, longword1);
    mask = __builtin_e2k_pcmpeqb (longword, 0);
    longword_ptr += 2;
    if (mask != 0) break;
  }
  /* Which of the bytes was the zero? */
  FIND_ZERO (longword0, longword1);
  tail = __builtin_ctzll (mask);
  dstp += (const char *) (longword_ptr - 2) - s1 + tail;

start_strncpy:
  /* почти strncpy в чистом виде, за исключением добавления последнего 0 */

  if (n <= OP_T_THRES) { /* малые длины */
    /* There are just a few bytes to copy.  Use byte memory operations.  */
    c = '\0';

#pragma noprefetch
#pragma loop count (7)
    while (n > 0) {
      *dstp++ = c = *srcp++;
      n--;
      if (c == '\0')
        break;
    }
    if (c != '\0')
      *dstp = '\0';
    return s1;
  }

  /* длины больше порога */
  end_ptr = E2K_ALIGN_PTR_BACK (dstp + n - 1, 8);

  if (end_ptr < dstp)  /* длины больше порога */
    end_ptr = (unsigned char *) (~0UL << 3);

  alignd = 8 - ((unsigned long int) dstp & 7);
  aligns = (srcp - dstp) & 7;
  n -= alignd;
  tail = n & 7;
  if (tail == 0) tail = 8;

  /* обработка до выровненного dest */
  dstp[0] = c = srcp[0];
  if (c == '\0')
    return s1;
  dstp[1] = c = srcp[1];
  if (c == '\0')
    return s1;
  dstp[2] = c = srcp[2];
  if (c == '\0')
    return s1;
  dstp[3] = c = srcp[3];
  if (c == '\0')
    return s1;
  dstp[4] = c = srcp[4];
  if (c == '\0')
    return s1;
  dstp[5] = c = srcp[5];
  if (c == '\0')
    return s1;
  dstp[6] = c = srcp[6];
  if (c == '\0')
    return s1;
  dstp[7] = c = srcp[7];
  if (c == '\0')
    return s1;
  srcp += alignd;
  dstp += alignd;

  if (aligns == 0) { /* обе строки выровнены одинаково */
    a0 = ((op_t *) srcp)[0];
    mask = __builtin_e2k_pcmpeqb (a0, 0);

    if (mask == 0) {

      /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
      for (;;) {
        ((op_t *) dstp)[0] = a0;
        srcp += OPSIZ;
        dstp += OPSIZ;
        a0 = ((op_t *) srcp)[0];
        mask = __builtin_e2k_pcmpeqb (a0, 0);
        if (!(mask == 0 && dstp < end_ptr)) break;
      }
    }
  }
  else { /* разное выравнивание строк */
    srcp = (unsigned char *) ((unsigned long int) srcp & ~7);
    E2K_PREPARE_ALIGN (aligns, spec);
    a00 = ((op_t *) srcp)[0];
    a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp, 8);
    E2K_ALIGN_DATA (a00, a01, a0, spec);
    mask = __builtin_e2k_pcmpeqb (a0, 0);

    if (mask == 0) {

      /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
#pragma unroll (1)
      for (;;) {
        ((op_t *) dstp)[0] = a0;
        srcp += OPSIZ;
        dstp += OPSIZ;
        a00 = a01;
        a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp, 8);
        E2K_ALIGN_DATA (a00, a01, a0, spec);
        mask = __builtin_e2k_pcmpeqb (a0, 0);
        if (mask != 0 || dstp >= end_ptr) break;
      }
    }
  }
  if (dstp < end_ptr) { /* нашли конец строки не в последнем дабле */
    tail = 8;
  }
  /* дописываем хвост */
  if ((mask & 0xffffffff) == 0 && tail >= 4) {
    *((int *) dstp) = a0;
    a0 >>= 32;
    mask >>= 32;
    dstp += 4;
    tail -= 4;
  }
  if ((mask & 0xffff) == 0 && tail >= 2) {
    *((short *) dstp) = a0;
    a0 >>= 16;
    mask >>= 16;
    dstp += 2;
    tail -= 2;
  }
  if ((mask & 0xff) == 0 && tail >= 1) {
    *dstp++ = a0;
    a0 >>= 8;
    mask >>= 8;
    tail--;
  }
  if ((mask & 0xff) == 0 && tail >= 1) {
    *dstp++ = a0;
  }
  *dstp = '\0';


#elif __iset__ <= 5

#define __CMP(mask, x) /* zero bytes -> bit mask */                       \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))

#define FIND_ZERO(qword0, qword1, qword2, qword3) {       \
  __CMP (mask0, qword0);                                  \
  __CMP (mask1, qword1);                                  \
  __CMP (mask2, qword2);                                  \
  __CMP (mask3, qword3);                                  \
  mask0 |= mask1 << 16;                                   \
  mask2 |= mask3 << 16;                                   \
  llmask = (mask2 << 32) | mask0;                         \
}

  op_t aligns, alignd;
  size_t align;
  __v2di a0, a00, a01, spec;
  const __v2di *qword_ptr, qzero = (__v2di) {0, 0};
  __v2di qword, qword0, qword1, qword2, qword3;
  unsigned long long llmask, mask0, mask1, mask2, mask3;
  unsigned int mask, count;

  if (n == 0) return s1;

  /* Find the end of the string.  */
  /*  dstp += strlen (s1); */
  /* почти strlen в чистом виде */

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (s1, 4096) > 4047, 0)) { /* closely to page border */
    /* Offsets 4048-4095 will be aligned into 4032 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (s1, 64);
    FIND_ZERO (qword_ptr[0], qword_ptr[1], qword_ptr[2], qword_ptr[3]);
    llmask >>= (s1 - (const char *) qword_ptr);

    if (llmask != 0) {
      dstp += __builtin_ctzll (llmask);
      goto start_strncpy;
    }
    qword_ptr += 4;
  }
  else {
    align = (size_t) s1 & 15;
    qword_ptr = E2K_ALIGN_PTR_BACK (s1, 16);

    /* first qword */
    qword = qword_ptr[0];
    __CMP (mask, qword);
    mask >>= align; /* обнуляем каждый байт до начала строки */

    if (mask != 0) {
      /* Which of the bytes was the zero? */
      dstp += __builtin_ctzll (mask);
      goto start_strncpy;
    }
    /* next 3 qwords */
    __CMP (mask1, qword_ptr[1]);
    __CMP (mask2, __builtin_e2k_ld_128_cleartag (qword_ptr, 32));
    __CMP (mask3, __builtin_e2k_ld_128_cleartag (qword_ptr, 48));
    mask2 |= mask3 << 16;
    llmask = (mask2 << 16) | mask1;

    if (llmask != 0) {
      /* Which of the bytes was the zero? */
      tail = __builtin_ctzll (llmask);
      dstp += (const char *) (qword_ptr + 1) - s1 + tail;
      goto start_strncpy;
    }
    qword_ptr = E2K_ALIGN_PTR_BACK (qword_ptr + 4, 64);
  }

  /* We will test a 64 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (8)
  for (;;) {
    qword0 = qword_ptr[0];
    qword1 = qword_ptr[1];
    qword2 = qword_ptr[2];
    qword3 = qword_ptr[3];
    qword = __builtin_e2k_qpminub (qword0, qword1);
    qword = __builtin_e2k_qpminub (qword, qword2);
    qword = __builtin_e2k_qpminub (qword, qword3);
    qword_ptr += 4;
    __CMP (mask, qword);
    if (mask != 0) break;
  }
  /* Which of the bytes was the zero? */
  FIND_ZERO (qword0, qword1, qword2, qword3);
  tail = __builtin_ctzll (llmask);
  dstp += (const char *) (qword_ptr - 4) - s1 + tail;

start_strncpy:
  /* почти strncpy в чистом виде, за исключением добавления последнего 0 */

  end_ptr = E2K_ALIGN_PTR_BACK (dstp + n - 1, 16);

  if ((dstp + n) < dstp)  /* длины больше порога */
    end_ptr = (unsigned char *) (~0UL << 4);

  alignd = 16 - ((unsigned long int) dstp & 15);
  aligns = (srcp - dstp) & 15;
  tail = (n - alignd) & 15;
  if (tail == 0) tail = 16;
  srcp += alignd;
  dstp += alignd;

  if (aligns == 0) { /* обе строки выровнены одинаково */
    a0 = ((__v2di *) srcp)[-1];
    __CMP (mask, a0);
    mask &= (-1 << (16 - alignd)); /* обнулили биты до начала dst */
    count = __builtin_ctz (mask);

    if (mask != 0 || dstp > end_ptr) { /* заканчиваем в этом qword */
      if (n <= count - (16 - alignd)) { /* n меньше длины строки */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << (16 - alignd + n)));
        dstp[-alignd + n] = '\0'; /* последний ноль */
      }
      else { /* записываем до конца строки, включая последний 0 */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-2 << count));
      }
      return s1;
    }

    __builtin_e2k_pst_128 (a0, dstp - 16, -1 << (16 - alignd));

    a0 = ((__v2di *) srcp)[0];
    __CMP (mask, a0);

    if (mask == 0 && dstp < end_ptr) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
      for (;;) {
        ((__v2di *) dstp)[0] = a0;
        srcp += 16;
        dstp += 16;
        a0 = ((__v2di *) srcp)[0];
        __CMP (mask, a0);
        if (!(mask == 0 && dstp < end_ptr)) break;
      }
    }
  }
  else { /* разное выравнивание строк */
    srcp = (unsigned char *) ((unsigned long int) srcp & ~15);
    E2K_PREPARE_ALIGN128 (aligns, spec);
    a00 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp, -16);
    a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp, 0);
    E2K_ALIGN_DATA128 (a00, a01, a0, spec);
    __CMP (mask, a0);
    mask &= (-1 << (16 - alignd)); /* обнулили биты до начала dst */
    count = __builtin_ctz (mask);

    if (mask != 0 || dstp > end_ptr) { /* заканчиваем в этом qword */
      if (n <= count - (16 - alignd)) { /* n меньше длины строки */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << (16 - alignd + n)));
        dstp[-alignd + n] = '\0'; /* последний ноль */
      }
      else { /* записываем до конца строки, включая последний 0 */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-2 << count));
      }
      return s1;
    }

    __builtin_e2k_pst_128 (a0, dstp - 16, -1 << (16 - alignd));

    a00 = a01;
    a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp, 16);
    E2K_ALIGN_DATA128 (a00, a01, a0, spec);
    __CMP (mask, a0);

    if (mask == 0 && dstp < end_ptr) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
#pragma unroll (1)
      for (;;) {
        ((__v2di *) dstp)[0] = a0;
        srcp += 16;
        dstp += 16;
        a00 = a01;
        a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp, 16);
        E2K_ALIGN_DATA128 (a00, a01, a0, spec);
        __CMP (mask, a0);
        if (mask != 0 || dstp >= end_ptr) break;
      }
    }
  }
  if (dstp < end_ptr) { /* нашли конец строки не в последнем qword */
    tail = 16;
  }
  /* дописываем хвост */
  count = __builtin_ctz (mask);
  if (count > tail) { /* хвост меньше длины строки */
    __builtin_e2k_pst_128 (a0, dstp, ~(-1 << tail));
    dstp[tail] = '\0'; /* последний ноль */
  }
  else { /* записываем до конца строки, включая последний 0 */
    __builtin_e2k_pst_128 (a0, dstp, ~(-2 << count));
  }


#else /* __iset__ > 5 */

#define __CMP(mask, x) /* zero bytes -> bit mask */                       \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __CMP_PRED(x)  /* zero bytes -> predicate */                      \
    __builtin_e2k_qpcmpeqbop (x, qzero)

#define FIND_ZERO(qword0, qword1) {                       \
  unsigned int mask0, mask1;                              \
  __CMP (mask0, qword0);                                  \
  __CMP (mask1, qword1);                                  \
  mask = (mask1 << 16) | mask0;                           \
}

  __v2di a0;
  size_t align;
  const __v2di *qword_ptr = (__v2di *) s1, qzero = (__v2di) {0, 0};
  __v2di qword, qword0, qword1;
  unsigned int mask, count;

  if (n == 0) return s1;

  /* Find the end of the string.  */
  /*  dstp += strlen (s1); */
  /* почти strlen в чистом виде */
  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (s1, 4096) > 4079, 0)) { /* closely to page border */
    /* Offsets 4080-4095 will be aligned into 4064 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (s1, 32);
    FIND_ZERO (qword_ptr[0], qword_ptr[1]);
    mask >>= (s1 - (const char *) qword_ptr);

    if (mask != 0) {
      dstp += __builtin_ctz (mask);
      goto start_strncpy;
    }
    qword_ptr += 2;
  }
  else {
    /* first qword load is unaligned */
    qword = ((__v2di *) s1)[0];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was the zero? */
      __CMP (mask, qword);
      dstp += __builtin_ctz (mask);
      goto start_strncpy;
    }
    /* next qword load is aligned */
    qword_ptr = E2K_ALIGN_PTR_BACK (s1, 16);
    qword = qword_ptr[1];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was the zero? */
      __CMP (mask, qword);
      tail = __builtin_ctz (mask);
      dstp += (const char *) (qword_ptr + 1) - s1 + tail;
      goto start_strncpy;
    }
    qword_ptr = E2K_ALIGN_PTR_BACK (qword_ptr + 2, 32);
  }

  /* We will test a 32 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (8)
  for (;;) {
    qword0 = qword_ptr[0];
    qword1 = qword_ptr[1];
    qword_ptr += 2;
    if (__CMP_PRED (qword0) || __CMP_PRED (qword1)) break;
  }
  /* Which of the bytes was the zero? */
  FIND_ZERO (qword0, qword1);
  tail = __builtin_ctz (mask);
  dstp += (const char *) (qword_ptr - 2) - s1 + tail;

start_strncpy:
  /* почти strncpy в чистом виде, за исключением добавления последнего 0 */

  align = (size_t) srcp & 15;
  end_ptr = dstp - align + ((n - 1 + align) & ~15);

  if ((dstp + n) < dstp)  /* длины больше порога */
    end_ptr = (unsigned char *) (~0UL << 4);

  tail = (n + align) & 15;
  if (tail == 0) tail = 16;
  srcp = (unsigned char *) ((size_t) srcp & ~15);
  dstp -= align;

  /* first qword load is aligned */
  a0 = ((__v2di *) srcp)[0];
  __CMP (mask, a0);
  mask &= (-1 << align); /* обнулили биты до начала src */
  count = __builtin_ctz (mask);

  if (mask != 0 || dstp >= end_ptr) { /* заканчиваем в этом qword */
    if (n <= count - align) { /* n меньше длины строки */
      __builtin_e2k_pst_128 (a0, dstp, (-1 << align) & ~(-1 << (align + n)));
      dstp[align + n] = '\0'; /* последний ноль */
    }
    else { /* записываем до конца строки, включая последний 0 */
      __builtin_e2k_pst_128 (a0, dstp, (-1 << align) & ~(-2 << count));
    }
    return s1;
  }

  __builtin_e2k_pst_128 (a0, dstp, -1 << align);

  srcp += 16;
  dstp += 16;
  /* next qword load is aligned */
  a0 = ((__v2di *) srcp)[0];

  if (__CMP_PRED (a0) == 0 && dstp < end_ptr) {

    /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
    for (;;) {
      ((__v2di *) dstp)[0] = a0;
      srcp += 16;
      dstp += 16;
      a0 = ((__v2di *) srcp)[0];
      if (!(__CMP_PRED (a0) == 0 && dstp < end_ptr)) break;
    }
  }
  if (dstp < end_ptr) { /* нашли конец строки не в последнем qword */
    tail = 16;
  }
  /* дописываем хвост */
  __CMP (mask, a0);
  count = __builtin_ctz (mask);
  if (count > tail) { /* хвост меньше длины строки */
    __builtin_e2k_pst_128 (a0, dstp, ~(-1 << tail));
    dstp[tail] = '\0'; /* последний ноль */
  }
  else { /* записываем до конца строки, включая последний 0 */
    __builtin_e2k_pst_128 (a0, dstp, ~(-2 << count));
  }

#endif /* __iset__ > 5 */

  return s1;
}
