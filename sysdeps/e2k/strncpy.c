/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strncpy.c 2218 2014-06-16 13:57:59Z vlog $
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
#include <memcopy.h>

#include <e2kintrin.h>

#undef strncpy

#ifndef STRNCPY
#define STRNCPY strncpy
#endif

#define restrict __restrict__

char *
STRNCPY (char * restrict s1, const char * restrict s2, size_t n)
{
  unsigned char * restrict srcp = (unsigned char *) s2;
  unsigned char * restrict dstp = (unsigned char *) s1;
  long int tail;

#if __iset__ <= 4

  op_t aligns, alignd;
  op_t a0, a00, a01, spec, mask;
  unsigned char * restrict end_ptr = E2K_ALIGN_PTR_BACK (dstp + n - 1, 8);
  unsigned char c;

  if (n <= OP_T_THRES) { /* малые длины */
    /* There are just a few bytes to copy.  Use byte memory operations.  */
#pragma loop count (4)
    while (n > 0) {
      *dstp++ = c = *srcp++;
      n--;
      if (c == '\0') {
#pragma loop count (4)
        while (n > 0) { /* заполняем нулями до конца */
          *dstp++ = '\0';
          n--;
        }
//FIXME: добавление этого return-а делает цикл в стр.102 1 тактным - беспредел
        return s1;
      }
    }
    return s1;
  }

  if (end_ptr < dstp)  /* длины больше порога */
    end_ptr = (unsigned char *) (~0UL << 3);

  alignd = E2K_BYTES_TO_ALIGN (dstp, 8);
  aligns = (srcp - dstp) & 7;
  n -= alignd;
  tail = n & 7;
  if (tail == 0) tail = 8;

#pragma loop count (2)
  while (alignd > 0) { /* копирование байт до выровненного dst или конца строки */
    *dstp++ = c = *srcp++;
    alignd--;
    if (c == '\0') {
#pragma loop count (2)
      while (alignd > 0) { /* заполняем нулями до выровненного dst */
        *dstp++ = '\0';
        alignd--;
      }
      goto fill_zero;
    }
  }

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
  __CTZB (spec, mask);
  if (mask != 0) { /* был конец строки в дабле */
    a0 &= ~((~0ULL) << (spec * 8)); /* обнуляем байты после конца строки */
  }

  if (dstp >= end_ptr) { /* заканчиваем в этом дабле */
    /* дописываем хвост */
    if (tail == 8) {
      ((op_t *) dstp)[0] = a0;
    }
    else {
      if (tail & 4) {
        *((int *) dstp) = a0;
        a0 >>= 32;
        dstp += 4;
      }
      if (tail & 2) {
        *((short *) dstp) = a0;
        a0 >>= 16;
        dstp += 2;
      }
      if (tail & 1) {
        *dstp = a0;
      }
    }
  }
  else { /* не последний дабл */
    ((op_t *) dstp)[0] = a0;
    dstp += OPSIZ;

fill_zero:
#pragma loop count (200)
    while (dstp + OPSIZ < end_ptr) { /* записываем нули по 2 дабла */
      ((op_t *) dstp)[0] = 0;
      ((op_t *) dstp)[1] = 0;
      dstp += 2 * OPSIZ;
    }
    if (dstp < end_ptr) { /* записываем ноль в еще 1 дабл */
      ((op_t *) dstp)[0] = 0;
      dstp += OPSIZ;
    }
    /* прописываем хвост нулями */
    if (tail == 8) {
      ((op_t *) dstp)[0] = 0;
    }
    else {
      if (tail & 4) {
        *((int *) dstp) = 0;
        dstp += 4;
      }
      if (tail & 2) {
        *((short *) dstp) = 0;
        dstp += 2;
      }
      if (tail & 1) {
        *dstp = '\0';
      }
    }
  }


#elif __iset__ <= 5

#define __CMP(mask, x) /* zero bytes -> bit mask */                       \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))

  op_t aligns, alignd;
  __v2di a0, a00, a01, spec;
  unsigned char * restrict end_ptr = E2K_ALIGN_PTR_BACK (dstp + n - 1, 16);
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  unsigned int mask, count;

  /* чтобы ничего не читать - при записи диагностики по 0 маске ломаемся */
  if (n == 0) return s1;

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

    if (dstp > end_ptr) { /* заканчиваем в этом qword */
      if (n < count - (16 - alignd)) /* n меньше длины строки */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << (16 - alignd + n)));
      else { /* записываем до конца строки, кроме последнего 0 */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << count));
        /* записываем 0 от конца строки до длины n */
        __builtin_e2k_pst_128 (qzero, dstp - 16, (-1 << count) & ~(-1 << (16 - alignd + n)));
      }
      return s1;
    }

    __builtin_e2k_pst_128 (a0, dstp - 16, -1 << (16 - alignd));

    if (mask != 0) { /* был конец строки в qword */
      /* обнуляем байты после конца строки */
      __builtin_e2k_pst_128 (qzero, dstp - 16, -1 << count);
      goto fill_zero;
    }

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
        a0 =  ((__v2di *) srcp)[0];
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

    if (dstp > end_ptr) { /* заканчиваем в этом qword */
      if (n < count - (16 - alignd)) /* n меньше длины строки */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << (16 - alignd + n)));
      else { /* записываем до конца строки, кроме последнего 0 */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << count));
        /* записываем 0 от конца строки до длины n */
        __builtin_e2k_pst_128 (qzero, dstp - 16, (-1 << count) & ~(-1 << (16 - alignd + n)));
      }
      return s1;
    }

    __builtin_e2k_pst_128 (a0, dstp - 16, -1 << (16 - alignd));

    if (mask != 0) { /* был конец строки в qword */
      /* обнуляем байты после конца строки */
      __builtin_e2k_pst_128 (qzero, dstp - 16, -1 << count);
      goto fill_zero;
    }

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
  count = __builtin_ctz (mask);
  if (dstp >= end_ptr) { /* заканчиваем в этом qword */
    /* дописываем хвост */
    __builtin_e2k_pst_128 (a0, dstp, ~(-1 << tail));
    if (count < tail) {
      /* записываем 0 от конца строки до длины n */
      __builtin_e2k_pst_128 (qzero, dstp, (-1 << count) & ~(-1 << tail));
    }
  }
  else { /* не последний qword */
    ((__v2di *) dstp)[0] = a0;
    __builtin_e2k_pst_128 (qzero, dstp, (-1 << count));
    dstp += 16;

fill_zero:
#pragma loop count (200)
    while (dstp + 16 < end_ptr) { /* записываем нули по 2 qword */
      ((__v2di *) dstp)[0] = qzero;
      ((__v2di *) dstp)[1] = qzero;
      dstp += 2 * 16;
    }
    if (dstp < end_ptr) { /* записываем ноль в еще 1 qword */
      ((__v2di *) dstp)[0] = qzero;
      dstp += 16;
    }
    /* прописываем хвост нулями */
    __builtin_e2k_pst_128 (qzero, dstp, ~(-1 << tail));
  }


#else /* __iset__ > 5 */

#define __CMP(mask, x) /* zero bytes -> bit mask */                       \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __CMP_PRED(x)  /* zero bytes -> predicate */                      \
    __builtin_e2k_qpcmpeqbop (x, qzero)

  __v2di a0;
  size_t align = (size_t) srcp & 15;
  unsigned char * restrict end_ptr = dstp - align + ((n - 1 + align) & ~15);
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  unsigned int mask, count;

  /* чтобы ничего не читать - при записи диагностики по 0 маске ломаемся */
  if (n == 0) return s1;

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

  if (dstp >= end_ptr) { /* заканчиваем в этом qword */
    if (n < count - align) /* n меньше длины строки */
      __builtin_e2k_pst_128 (a0, dstp, (-1 << align) & ~(-1 << (align + n)));
    else { /* записываем до конца строки, кроме последнего 0 */
      __builtin_e2k_pst_128 (a0, dstp, (-1 << align) & ~(-1 << count));
      /* записываем 0 от конца строки до длины n */
      __builtin_e2k_pst_128 (qzero, dstp, (-1 << count) & ~(-1 << (align + n)));
    }
    return s1;
  }

  __builtin_e2k_pst_128 (a0, dstp, -1 << align);

  if (mask != 0) { /* был конец строки в qword */
    /* обнуляем байты после конца строки */
    __builtin_e2k_pst_128 (qzero, dstp, -1 << count);
    dstp += 16;
    goto fill_zero;
  }

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

  /* Which of the bytes was the zero? */
  __CMP (mask, a0);
  count = __builtin_ctz (mask);
  if (dstp >= end_ptr) { /* заканчиваем в этом qword */
    /* дописываем хвост */
    __builtin_e2k_pst_128 (a0, dstp, ~(-1 << tail));
    if (count < tail) {
      /* записываем 0 от конца строки до длины n */
      __builtin_e2k_pst_128 (qzero, dstp, (-1 << count) & ~(-1 << tail));
    }
  }
  else { /* не последний qword */
    ((__v2di *) dstp)[0] = a0;
    __builtin_e2k_pst_128 (qzero, dstp, (-1 << count));
    dstp += 16;

fill_zero:
#pragma loop count (200)
    while (dstp + 16 < end_ptr) { /* записываем нули по 2 qword */
      ((__v2di *) dstp)[0] = qzero;
      ((__v2di *) dstp)[1] = qzero;
      dstp += 2 * 16;
    }
    if (dstp < end_ptr) { /* записываем ноль в еще 1 qword */
      ((__v2di *) dstp)[0] = qzero;
      dstp += 16;
    }
    /* прописываем хвост нулями */
    __builtin_e2k_pst_128 (qzero, dstp, ~(-1 << tail));
  }

#endif /* __iset__ > 5 */

  return s1;
}

libc_hidden_builtin_def (strncpy)
