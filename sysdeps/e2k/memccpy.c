/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: memccpy.c 5560 2020-03-01 08:29:24Z vlog $
 */

/* Copyright (C) 1991-2015 Free Software Foundation, Inc.
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

//#undef E2K_PREPARE_ALIGN
//#undef E2K_ALIGN_DATA
///* вариант на операциях insfd/scrd для данных формата 64 */
//#define E2K_PREPARE_ALIGN(align, spec) spec = align << 9
//#define E2K_ALIGN_DATA(src1, src2, dst, spec)             \
//    dst = __builtin_e2k_scrd (__builtin_e2k_insfd (src1, spec, src2), spec >> 6)

#undef __memccpy
#undef memccpy

#define restrict __restrict__

/* Copy no more than N bytes of SRC to DEST, stopping when C is found.
   Return the position in DEST one byte past where C was copied, or
   NULL if C was not found in the first N bytes of SRC.  */
void *
__memccpy (void * restrict dest, const void * restrict src, int c_in, size_t n)
{
  unsigned char * restrict srcp = (unsigned char *) src;
  unsigned char * restrict dstp = (unsigned char *) dest;
  op_t charmask = __builtin_e2k_pshufb (c_in, c_in, 0);
  long int tail;

#if __iset__ <= 4

  op_t aligns, alignd, a0, a00, a01, spec, mask;
  unsigned char * restrict end_ptr = E2K_ALIGN_PTR_BACK (dstp + n - 1, 8);
  unsigned char s, c = (unsigned char) c_in;

  if (n <= OP_T_THRES) { /* малые длины */
    /* There are just a few bytes to copy.  Use byte memory operations.  */
#pragma noprefetch
#pragma loop count (8)
    while (n > 0) {
      *dstp++ = s = *srcp++;
      n--;
      if (s == c) {
         return (char *) dstp;
      }
    }
    return NULL;
  }

  if (end_ptr < dstp)  /* длины больше порога */
    end_ptr = (unsigned char *) (~0UL << 3);

  alignd = 8 - ((unsigned long int) dstp & 7);
  aligns = (srcp - dstp) & 7;
  n -= alignd;
  tail = n & 7;
  if (tail == 0) tail = 8;

  /* обработка до выровненного dest */
  dstp[0] = s = srcp[0];
  if (s == c)
      return (char *) dstp + 1;
  dstp[1] = s = srcp[1];
  if (s == c)
      return (char *) dstp + 2;
  dstp[2] = s = srcp[2];
  if (s == c)
      return (char *) dstp + 3;
  dstp[3] = s = srcp[3];
  if (s == c)
      return (char *) dstp + 4;
  dstp[4] = s = srcp[4];
  if (s == c)
      return (char *) dstp + 5;
  dstp[5] = s = srcp[5];
  if (s == c)
      return (char *) dstp + 6;
  dstp[6] = s = srcp[6];
  if (s == c)
      return (char *) dstp + 7;
  dstp[7] = s = srcp[7];
  if (s == c)
      return (char *) dstp + 8;
  srcp += alignd;
  dstp += alignd;

  if (aligns == 0) { /* обе строки выровнены одинаково */

    a0 = ((op_t *) srcp)[0];
    mask = __builtin_e2k_pcmpeqb (a0, charmask);

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
        mask = __builtin_e2k_pcmpeqb (a0, charmask);
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
    mask = __builtin_e2k_pcmpeqb (a0, charmask);

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
        mask = __builtin_e2k_pcmpeqb (a0, charmask);
        if (mask != 0 || dstp >= end_ptr) break;
      }
    }
  }
  if (dstp < end_ptr) { /* нашли символ не в последнем дабле */
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
    tail--;
  }
  if (tail > 0) {
    *dstp = a0;
    if ((a0 & 0xff) == c)
      return dstp + 1;
  }


#elif __iset__ <= 5

#define __CMP(mask, x) /* bytes with 'c_in' -> bit mask */                    \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))

  op_t aligns, alignd;
  __v2di a0, a00, a01, spec;
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  unsigned char * restrict end_ptr = E2K_ALIGN_PTR_BACK (dstp + n - 1, 16);
  unsigned int mask;

  if (n == 0) return NULL;

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

    if (mask != 0 || dstp > end_ptr) { /* заканчиваем в этом qword */
      mask = __builtin_ctz (mask);
      if (n <= mask - (16 - alignd)) { /* n меньше длины строки */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << (16 - alignd + n)));
        return NULL;
      }
      else { /* записываем до конца строки, включая последний c_in */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-2 << mask));
        return (char *) dstp - 16 + mask + 1;
      }
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

    if (mask != 0 || dstp > end_ptr) { /* заканчиваем в этом qword */
      mask = __builtin_ctz (mask);
      if (n <= mask - (16 - alignd)) { /* n меньше длины строки */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-1 << (16 - alignd + n)));
        return NULL;
      }
      else { /* записываем до конца строки, включая последний c_in */
        __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-2 << mask));
        return (char *) dstp - 16 + mask + 1;
      }
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
  if (dstp >= end_ptr) { /* дошли до последнего qword */
    mask &= ~(-1 << tail); /* обнуляем биты после последнего байта для записи */
  }
  /* дописываем хвост */
  if (mask != 0) { /* нашли символ в последнем qword */
    mask = __builtin_ctz (mask);
    __builtin_e2k_pst_128 (a0, dstp, ~(-2 << mask));
    return (char *) dstp + mask + 1;
  }
  __builtin_e2k_pst_128 (a0, dstp, ~(-1 << tail));


#else /* __iset__ > 5 */

#define __CMP(mask, x) /* bytes with 'c_in' -> bit mask */                    \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))
#define __CMP_PRED(x)  /* bytes with 'c_in' -> predicate */                   \
    __builtin_e2k_qpcmpeqbop (x, qcharmask)

  __v2di a0;
  size_t align = (size_t) srcp & 15;
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  unsigned char * restrict end_ptr = dstp - align + ((n - 1 + align) & ~15);
  unsigned int mask;

  if (n == 0) return NULL;

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

  if (mask != 0 || dstp >= end_ptr) { /* заканчиваем в этом qword */
    mask = __builtin_ctz (mask);
    if (n <= mask - align) { /* n меньше длины строки */
      __builtin_e2k_pst_128 (a0, dstp, (-1 << align) & ~(-1 << (align + n)));
      return NULL;
    }
    else { /* записываем до конца строки, включая последний c_in */
      __builtin_e2k_pst_128 (a0, dstp, (-1 << align) & ~(-2 << mask));
      return (char *) dstp + mask + 1;
    }
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
      if (__CMP_PRED (a0) || dstp >= end_ptr) break;
    }
  }
  /* Which of the bytes was the zero? */
  __CMP (mask, a0);

  if (dstp >= end_ptr) { /* дошли до последнего qword */
    mask &= ~(-1 << tail); /* обнуляем биты после последнего байта для записи */
  }
  /* дописываем хвост */
  if (mask != 0) { /* нашли символ в последнем qword */
    mask = __builtin_ctz (mask);
    __builtin_e2k_pst_128 (a0, dstp, ~(-2 << mask));
    return (char *) dstp + mask + 1;
  }
  __builtin_e2k_pst_128 (a0, dstp, ~(-1 << tail));

#endif /* __iset__ > 5 */

  return NULL;
}

weak_alias (__memccpy, memccpy)
