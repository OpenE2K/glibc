/* Copyright (c) 2015-2021 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strcat.c 2226 2014-06-16 17:46:19Z vlog $
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

#undef E2K_PREPARE_ALIGN
#undef E2K_ALIGN_DATA
/* вариант на операциях insfd/scrd для данных формата 64 */
#define E2K_PREPARE_ALIGN(align, spec) spec = align << 9
#define E2K_ALIGN_DATA(src1, src2, dst, spec)             \
    dst = __builtin_e2k_scrd (__builtin_e2k_insfd (src1, spec, src2), spec >> 6)

#undef strcat

#define restrict __restrict__

/* Append SRC on the end of DEST.  */
char *
strcat (dest, src)
     char * restrict dest;
     const char * restrict src;
{
  unsigned char * restrict srcp = (unsigned char *) src;
  unsigned char * restrict dstp = (unsigned char *) dest;
  size_t align, tail;

#if __iset__ <= 4

#define FIND_ZERO(longword0, longword1) {                 \
  unsigned long long mask0, mask1;                        \
  mask0 = __builtin_e2k_pcmpeqb (longword0, 0);           \
  mask1 = __builtin_e2k_pcmpeqb (longword1, 0);           \
  mask = __builtin_e2k_pmovmskb (mask1, mask0);           \
}

  op_t aligns, alignd;
  op_t a0, a00, a01, spec, mask;
  const unsigned long long *longword_ptr;
  unsigned long long longword, longword0, longword1;
  unsigned char c;

  /* Find the end of the string.  */
  /*  dstp += strlen (dest); */
  /* почти strlen в чистом виде */

  align = (size_t) dest & 7;
  longword_ptr = E2K_ALIGN_PTR_BACK (dest, 8);

  /* first longword */
  longword = longword_ptr[0];
  mask = __builtin_e2k_pcmpeqb (longword, 0) >> (8 * align);

  if (mask != 0) {
    /* Which of the bytes was the zero? */
    __CTZB (tail, mask);
    dstp += tail;
    goto start_strcpy;
  }

  /* second longword */
  longword = longword_ptr[1];
  mask = __builtin_e2k_pcmpeqb (longword, 0);

  if (mask != 0) {
    /* Which of the bytes was the zero? */
    __CTZB (tail, mask);
    dstp += (const char *) (longword_ptr + 1) - dest + tail;
    goto start_strcpy;
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
  dstp += (const char *) (longword_ptr - 2) - dest + tail;

start_strcpy:
  /* почти strcpy в чистом виде */

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
  __v2di a0, a00, a01, spec;
  const __v2di *qword_ptr, qzero = (__v2di) {0, 0};
  __v2di qword, qword0, qword1, qword2, qword3;
  unsigned long long llmask, mask0, mask1, mask2, mask3;
  unsigned int mask;

  /* Find the end of the string.  */
  /*  dstp += strlen (dest); */
  /* почти strlen в чистом виде */

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (dest, 4096) > 4047, 0)) { /* closely to page border */
    /* Offsets 4048-4095 will be aligned into 4032 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (dest, 64);
    FIND_ZERO (qword_ptr[0], qword_ptr[1], qword_ptr[2], qword_ptr[3]);
    llmask >>= (dest - (const char *) qword_ptr);

    if (llmask != 0) {
      dstp += __builtin_ctzll (llmask);
      goto start_strcpy;
    }
    qword_ptr += 4;
  }
  else {
    align = (size_t) dest & 15;
    qword_ptr = E2K_ALIGN_PTR_BACK (dest, 16);

    /* first qword */
    qword = qword_ptr[0];
    __CMP (mask, qword);
    mask >>= align; /* обнуляем каждый байт до начала строки */

    if (mask != 0) {
      /* Which of the bytes was the zero? */
      dstp += __builtin_ctzll (mask);
      goto start_strcpy;
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
      dstp += (const char *) (qword_ptr + 1) - dest + tail;
      goto start_strcpy;
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
  dstp += (const char *) (qword_ptr - 4) - dest + tail;

start_strcpy:
  /* почти strcpy в чистом виде */

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

#define FIND_ZERO(qword0, qword1) {                       \
  unsigned int mask0, mask1;                              \
  __CMP (mask0, qword0);                                  \
  __CMP (mask1, qword1);                                  \
  mask = (mask1 << 16) | mask0;                           \
}

  __v2di a0;
  const __v2di *qword_ptr, qzero = (__v2di) {0, 0};
  __v2di qword, qword0, qword1;
  unsigned int mask;

  /* Find the end of the string.  */
  /*  dstp += strlen (dest); */
  /* почти strlen в чистом виде */

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (dest, 4096) > 4079, 0)) { /* closely to page border */
    /* Offsets 4080-4095 will be aligned into 4064 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (dest, 32);
    FIND_ZERO (qword_ptr[0], qword_ptr[1]);
    mask >>= (dest - (const char *) qword_ptr);

    if (mask != 0) {
      dstp += __builtin_ctz (mask);
      goto start_strcpy;
    }
    qword_ptr += 2;
  }
  else {
    /* first qword load is unaligned */
    qword = ((__v2di *) dest)[0];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was the zero? */
      __CMP (mask, qword);
      dstp += __builtin_ctz (mask);
      goto start_strcpy;
    }
    /* next qword load is aligned */
    qword_ptr = E2K_ALIGN_PTR_BACK (dest, 16);
    qword = qword_ptr[1];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was the zero? */
      __CMP (mask, qword);
      tail = __builtin_ctz (mask);
      dstp += (const char *) (qword_ptr + 1) - dest + tail;
      goto start_strcpy;
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
  dstp += (const char *) (qword_ptr - 2) - dest + tail;

start_strcpy:
  /* почти strcpy в чистом виде */

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

    if (__CMP_PRED (a0)) {
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

libc_hidden_builtin_def (strcat)
