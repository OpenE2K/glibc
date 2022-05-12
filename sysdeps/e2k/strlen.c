/* Copyright (c) 2015-2021 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strlen.c 2068 2014-04-23 11:56:36Z vlog $
 */

#include <string.h>
#include <stdlib.h>
#include <e2kintrin.h>

#undef strlen

/* Return the length of the null-terminated string STR.  Scan for
   the null terminator quickly by testing eight bytes at a time.  */

size_t
strlen (const char *str)
{
  size_t tail;

#if __iset__ <= 4

#define FIND_ZERO(longword0, longword1) {                 \
  unsigned long long mask0, mask1;                        \
  mask0 = __builtin_e2k_pcmpeqb (longword0, 0);           \
  mask1 = __builtin_e2k_pcmpeqb (longword1, 0);           \
  mask = __builtin_e2k_pmovmskb (mask1, mask0);           \
}

  if (str[0] == '\0')
    return 0;
  if (str[1] == '\0')
    return 1;
  if (str[2] == '\0')
    return 2;
  if (str[3] == '\0')
    return 3;
  if (str[4] == '\0')
    return 4;
  if (str[5] == '\0')
    return 5;
  if (str[6] == '\0')
    return 6;
  if (str[7] == '\0')
    return 7;

  const unsigned long long *longword_ptr;
  unsigned long long longword, longword0, longword1, mask;

  longword_ptr = E2K_ALIGN_PTR_BACK (str, 8);

  /* first longword */
  longword = longword_ptr[1];
  mask = __builtin_e2k_pcmpeqb (longword, 0);

  if (mask != 0) {
    /* Which of the bytes was the zero? */
    __CTZB (tail, mask);
    return (const char *) (longword_ptr + 1) - str + tail;
  }
  longword_ptr = E2K_ALIGN_PTR_BACK (longword_ptr + 2, 16);

  /* откручиваем одну итерацию */
  longword0 = longword_ptr[0];
  longword1 = longword_ptr[1];
  longword = __builtin_e2k_pminub (longword0, longword1);
  mask = __builtin_e2k_pcmpeqb (longword, 0);
  if (mask != 0) {
    /* Which of the bytes was the zero? */
    FIND_ZERO (longword0, longword1);
    tail = __builtin_ctzll (mask);
    return (const char *) longword_ptr - str + tail;
  }
  longword_ptr += 2;

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
  return (const char *) (longword_ptr - 2) - str + tail;


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
  mask = (mask2 << 32) | mask0;                           \
}

  const __v2di *qword_ptr, qzero = (__v2di) {0, 0};
  __v2di qword, qword0, qword1, qword2, qword3;
  unsigned long long mask, mask0, mask1, mask2, mask3;
  unsigned int align;

  if (str[0] == '\0')
      return 0;
  if (str[1] == '\0')
      return 1;
  if (str[2] == '\0')
      return 2;
  if (str[3] == '\0')
      return 3;
  if (str[4] == '\0')
      return 4;
  if (str[5] == '\0')
      return 5;
  if (str[6] == '\0')
      return 6;
  if (str[7] == '\0')
      return 7;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4047, 0)) { /* closely to page border */
    /* Offsets 4048-4095 will be aligned into 4032 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 64);
    FIND_ZERO (qword_ptr[0], qword_ptr[1], qword_ptr[2], qword_ptr[3]);
    mask >>= (str - (const char *) qword_ptr);

    if (mask != 0)
      return __builtin_ctzll (mask);
    qword_ptr += 4;
  }
  else {
    align = (size_t) str & 15;
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);

    /* first qword */
    qword = qword_ptr[0];
    __CMP (mask, qword);
    mask >>= align; /* обнуляем каждый байт до начала строки */

    if (mask != 0) {
      /* Which of the bytes was the zero? */
      return __builtin_ctzll (mask);
    }
    /* next 3 qwords */
    __CMP (mask1, qword_ptr[1]);
    __CMP (mask2, __builtin_e2k_ld_128_cleartag (qword_ptr, 32));
    __CMP (mask3, __builtin_e2k_ld_128_cleartag (qword_ptr, 48));
    mask2 |= mask3 << 16;
    mask = (mask2 << 16) | mask1;

    if (mask != 0) {
      /* Which of the bytes was the zero? */
      tail = __builtin_ctzll (mask);
      return (const char *) (qword_ptr + 1) - str + tail;
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
  tail = __builtin_ctzll (mask);
  return (const char *) (qword_ptr - 4) - str + tail;


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

  const __v2di *qword_ptr, qzero = (__v2di) {0, 0};
  __v2di qword, qword0, qword1;
  unsigned int mask;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4079, 0)) { /* closely to page border */
    /* Offsets 4080-4095 will be aligned into 4064 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 32);
    FIND_ZERO (qword_ptr[0], qword_ptr[1]);
    mask >>= (str - (const char *) qword_ptr);

    if (mask != 0)
      return __builtin_ctz (mask);
    qword_ptr += 2;
  }
  else {
    /* first qword load is unaligned */
    qword = ((__v2di *) str)[0];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was the zero? */
      __CMP (mask, qword);
      return __builtin_ctz (mask);
    }
    /* next qword load is aligned */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);
    qword = qword_ptr[1];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was the zero? */
      __CMP (mask, qword);
      tail = __builtin_ctz (mask);
      return (const char *) (qword_ptr + 1) - str + tail;
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
  return (const char *) (qword_ptr - 2) - str + tail;

#endif /* __iset__ > 5 */

}

libc_hidden_builtin_def (strlen)
