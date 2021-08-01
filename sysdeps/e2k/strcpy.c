/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strcpy.c 2101 2014-05-13 11:24:32Z vlog $
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

#include <stddef.h>
#include <string.h>
#include <memcopy.h>
#include <e2kintrin.h>

#undef strcpy

#define restrict __restrict__

/* Copy SRC to DEST.  */
char *
strcpy (dest, src)
     char * restrict dest;
     const char * restrict src;
{
  unsigned char * restrict srcp = (unsigned char *) src;
  unsigned char * restrict dstp = (unsigned char *) dest;
  unsigned char c;

#if __iset__ <= 4

  op_t aligns, alignd;
  op_t a0, a00, a01, spec, mask;

  /* обработка до выровненного dest */
  dstp[0] = c = srcp[0];
  if (c == '\0')
      return dest;
  dstp[1] = c = srcp[1];
  if (c == '\0')
      return dest;
  dstp[2] = c = srcp[2];
  if (c == '\0')
      return dest;
  dstp[3] = c = srcp[3];
  if (c == '\0')
      return dest;
  dstp[4] = c = srcp[4];
  if (c == '\0')
      return dest;
  dstp[5] = c = srcp[5];
  if (c == '\0')
      return dest;
  dstp[6] = c = srcp[6];
  if (c == '\0')
      return dest;
  dstp[7] = c = srcp[7];
  if (c == '\0')
      return dest;

  alignd = 8 - ((unsigned long int) dstp & 7);
  aligns = (srcp - dstp) & 7;
  srcp += alignd;
  dstp += alignd;

  if (aligns == 0) { /* обе строки выровнены одинаково */
    a0 = ((op_t *) srcp)[0];
    mask = __builtin_e2k_pcmpeqb (a0, 0);

    if (mask == 0) {

      /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (1000)
      for (;;) {
        ((op_t *) dstp)[0] = a0;
        srcp += OPSIZ;
        dstp += OPSIZ;
        a0 = ((op_t *) srcp)[0];
        mask = __builtin_e2k_pcmpeqb (a0, 0);
        if (mask != 0) break;
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
#pragma loop count (1000)
      for (;;) {
        ((op_t *) dstp)[0] = a0;
        srcp += OPSIZ;
        dstp += OPSIZ;
        a00 = a01;
        a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp, 8);
        E2K_ALIGN_DATA (a00, a01, a0, spec);
        mask = __builtin_e2k_pcmpeqb (a0, 0);
        if (mask != 0) break;
      }
    }
  }
  /* обработка хвоста */
  if ((mask & 0xffffffff) == 0) {
    *((int *) dstp) = a0;
    a0 >>= 32;
    mask >>= 32;
    dstp += 4;
  }
  if ((mask & 0xffff) == 0) {
    *((short *) dstp) = a0;
    a0 >>= 16;
    mask >>= 16;
    dstp += 2;
  }
  if ((mask & 0xff) == 0) {
    *dstp = a0;
    dstp++;
  }
  *dstp = '\0';


#elif __iset__ <= 5

#define __CMP(mask, x) /* zero bytes -> bit mask */                       \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))

  op_t aligns, alignd;
  __v2di a0, a00, a01, spec;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  unsigned int mask;

  /* обработка первых 2-х байт */
  dstp[0] = c = srcp[0];
  if (c == '\0')
      return dest;
  dstp[1] = c = srcp[1];
  if (c == '\0')
      return dest;

  alignd = 16 - ((unsigned long int) dstp & 15);
  aligns = (srcp - dstp) & 15;
  srcp += alignd;
  dstp += alignd;

  if (aligns == 0) { /* обе строки выровнены одинаково */

    a0 = ((__v2di *) srcp)[-1];
    __CMP (mask, a0);
    mask &= (-1 << (16 - alignd)); /* обнулили биты до начала dst */

    if (mask != 0) {
      /* Which of the bytes was the zero? */
      mask = __builtin_ctz (mask);
      __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-2 << mask));
      return dest;
    }
    __builtin_e2k_pst_128 (a0, dstp - 16, -1 << (16 - alignd));

    a0 = ((__v2di *) srcp)[0];
    __CMP (mask, a0);

    if (mask == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (1000)
      for (;;) {
        ((__v2di *) dstp)[0] = a0;
        srcp += 16;
        dstp += 16;
        a0 = ((__v2di *) srcp)[0];
        __CMP (mask, a0);
        if (mask != 0) break;
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

    if (mask != 0) {
      /* Which of the bytes was the zero? */
      mask = __builtin_ctz (mask);
      __builtin_e2k_pst_128 (a0, dstp - 16, (-1 << (16 - alignd)) & ~(-2 << mask));
      return dest;
    }
    __builtin_e2k_pst_128 (a0, dstp - 16, -1 << (16 - alignd));

    a00 = a01;
    a01 =__builtin_e2k_ld_128_cleartag ((__v2di *) srcp, 16);
    E2K_ALIGN_DATA128 (a00, a01, a0, spec);
    __CMP (mask, a0);

    if (mask == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (1000)
      for (;;) {
        ((__v2di *) dstp)[0] = a0;
        srcp += 16;
        dstp += 16;
        a00 = a01;
//        a01 = ((__v2di *) srcp)[1];
        a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp, 16);
        E2K_ALIGN_DATA128 (a00, a01, a0, spec);
        __CMP (mask, a0);
        if (mask != 0) break;
      }
    }
  }
  /* обработка хвоста */
  /* Which of the bytes was the zero? */
  mask = __builtin_ctz (mask);
  __builtin_e2k_pst_128 (a0, dstp, ~(-2 << mask));


#else /* __iset__ > 5 */

#define __CMP(mask, x) /* zero bytes -> bit mask */                       \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __CMP_PRED(x)  /* zero bytes -> predicate */                      \
    __builtin_e2k_qpcmpeqbop (x, qzero)

  __v2di a0;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  size_t align;
  unsigned int mask;

  /* обработка первых 4-х байт */
  dstp[0] = c = srcp[0];
  if (c == '\0')
      return dest;
  dstp[1] = c = srcp[1];
  if (c == '\0')
      return dest;
  dstp[2] = c = srcp[2];
  if (c == '\0')
      return dest;
  dstp[3] = c = srcp[3];
  if (c == '\0')
      return dest;
  srcp += 4;
  dstp += 4;

  align = (size_t) srcp & 15;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (srcp, 4096) > 4080, 0)) { /* closely to page border */
    /* Offsets 4081-4095 will be shifted back to the aligned address thus fit into page */
    srcp = (unsigned char *) ((size_t) srcp & ~15);
    dstp -= align;

    /* first qword load is aligned */
    a0 = ((__v2di *) srcp)[0];
    __CMP (mask, a0);
    mask &= (-1 << align); /* обнулили биты до начала src */

    if (mask != 0) {
      /* Which of the bytes was the zero? */
      mask = __builtin_ctz (mask);
      __builtin_e2k_pst_128 (a0, dstp, (-1 << align) & ~(-2 << mask));
      return dest;
    }
    __builtin_e2k_pst_128 (a0, dstp, -1 << align);
    srcp += 16;
    dstp += 16;
  }
  else {
    /* first qword load is unaligned */
    a0 = ((__v2di *) srcp)[0];

    if (__CMP_PRED (a0) != 0) {
      /* Which of the bytes was the zero? */
      __CMP (mask, a0);
      mask = __builtin_ctz (mask);
      __builtin_e2k_pst_128 (a0, dstp, ~(-2 << mask));
      return dest;
    }
    __builtin_e2k_pst_128 (a0, dstp, 0xffff >> align);
    srcp += 16 - align;
    dstp += 16 - align;
  }

  /* next qword load is aligned */
  a0 = ((__v2di *) srcp)[0];

  if (__CMP_PRED (a0) == 0) {

    /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (1000)
#pragma unroll (1)
    for (;;) {
      ((__v2di *) dstp)[0] = a0;
      srcp += 16;
      dstp += 16;
      a0 = ((__v2di *) srcp)[0];
      if (__CMP_PRED (a0)) break;
    }
  }
  /* обработка хвоста */
  /* Which of the bytes was the zero? */
  __CMP (mask, a0);
  mask = __builtin_ctz (mask);
  __builtin_e2k_pst_128 (a0, dstp, ~(-2 << mask));

#endif /* __iset__ > 5 */

  return dest;
}

libc_hidden_builtin_def (strcpy)
