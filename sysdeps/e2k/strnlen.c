/* Copyright (c) 2015-2021 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strnlen.c 2202 2014-06-10 16:05:22Z vlog $
 */

#include <string.h>
#include <stdlib.h>

#include <e2kintrin.h>

/* Find the length of S, but scan at most MAXLEN characters.  If no
   '\0' terminator is found in that many characters, return MAXLEN.  */

#ifdef STRNLEN
# define __strnlen STRNLEN
#endif

size_t
__strnlen (const char *str, size_t maxlen)
{
  const char *end_ptr = str + maxlen;
  size_t tail;

  if (end_ptr < str)
    end_ptr = (const char *) ~0UL;

#if __iset__ <= 4

#define FIND_ZERO(longword0, longword1, longword2, longword3) { \
  mask0 = __builtin_e2k_pcmpeqb (longword0, 0);                 \
  mask1 = __builtin_e2k_pcmpeqb (longword1, 0);                 \
  mask2 = __builtin_e2k_pcmpeqb (longword2, 0);                 \
  mask3 = __builtin_e2k_pcmpeqb (longword3, 0);                 \
  mask = __builtin_e2k_pmovmskb (mask1, mask0);                 \
  mask |= __builtin_e2k_pmovmskb (mask3, mask2) << 16;          \
}

  const unsigned long long *longword_ptr;
  unsigned long long longword, longword0, longword1, longword2, longword3;
  unsigned long long mask0, mask1, mask2, mask3;
  unsigned int mask;

  if (maxlen == 0 || str[0] == '\0')
    return 0;
  if (maxlen == 1 || str[1] == '\0')
    return 1;
  if (maxlen == 2 || str[2] == '\0')
    return 2;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4071, 0)) { /* closely to page border */
    /* Offsets 4072-4095 will be aligned into 4064 thus fit into page */
    longword_ptr = E2K_ALIGN_PTR_BACK (str, 32);
  }
  else {
    longword_ptr = E2K_ALIGN_PTR_BACK (str, 8);
  }
  FIND_ZERO (longword_ptr[0],
	     __builtin_e2k_ld_64s_cleartag (longword_ptr, 8),
	     __builtin_e2k_ld_64s_cleartag (longword_ptr, 16),
	     __builtin_e2k_ld_64s_cleartag (longword_ptr, 24));
  mask >>= (str - (const char *) longword_ptr);
  longword_ptr += 4;

  if (mask != 0 || (const char *) longword_ptr >= end_ptr) {
    /* Which of the bytes was the zero? */
    tail = __builtin_ctz (mask);
    return tail > maxlen ? maxlen : tail;
  }
  longword_ptr = E2K_ALIGN_PTR_BACK (longword_ptr, 32);

  /* We will test a 32 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (8)
  for (;;) {
    longword0 = longword_ptr[0];
    longword1 = longword_ptr[1];
    longword2 = longword_ptr[2];
    longword3 = longword_ptr[3];
    longword = __builtin_e2k_pminub (longword0, longword1);
    longword = __builtin_e2k_pminub (longword, longword2);
    longword = __builtin_e2k_pminub (longword, longword3);
    mask0 = __builtin_e2k_pcmpeqb (longword, 0);
    longword_ptr += 4;
    if (mask0 != 0 || (const char *) longword_ptr >= end_ptr) break;
  }
  /* Which of the bytes was the zero? */
  FIND_ZERO (longword0, longword1, longword2, longword3);
  tail = __builtin_ctz (mask);
  tail += (const char *) (longword_ptr - 4) - str;


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

  if (maxlen == 0 || str[0] == '\0')
    return 0;
  if (maxlen == 1 || str[1] == '\0')
    return 1;
  if (maxlen == 2 || str[2] == '\0')
    return 2;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4047, 0)) { /* closely to page border */
    /* Offsets 4048-4095 will be aligned into 4032 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 64);
  }
  else {
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);
  }
  FIND_ZERO (qword_ptr[0],
	     __builtin_e2k_ld_128_cleartag (qword_ptr, 16),
	     __builtin_e2k_ld_128_cleartag (qword_ptr, 32),
	     __builtin_e2k_ld_128_cleartag (qword_ptr, 48));
  mask >>= (str - (const char *) qword_ptr);
  qword_ptr += 4;

  if (mask != 0 || (const char *) qword_ptr >= end_ptr) {
    /* Which of the bytes was the zero? */
    tail = __builtin_ctzll (mask);
    return tail > maxlen ? maxlen : tail;
  }
  qword_ptr = E2K_ALIGN_PTR_BACK (qword_ptr, 64);

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
    __CMP (mask, qword);
    qword_ptr += 4;
    if (mask != 0 || (const char *) qword_ptr >= end_ptr) break;
  }
  /* Which of the bytes was the zero? */
  FIND_ZERO (qword0, qword1, qword2, qword3);
  tail = __builtin_ctzll (mask);
  tail += (const char *) (qword_ptr - 4) - str;


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
  __v2di qword0, qword1;
  unsigned int mask;

  if (maxlen == 0) return 0;

  qword_ptr = E2K_ALIGN_PTR_BACK (str, 32);
  FIND_ZERO (qword_ptr[0], qword_ptr[1]);
  mask >>= (str - (const char *) qword_ptr);
  qword_ptr += 2;

  if (mask != 0 || (const char *) qword_ptr >= end_ptr) {
    /* Which of the bytes was the zero? */
    tail = __builtin_ctz (mask);
    return tail > maxlen ? maxlen : tail;
  }

  /* откручиваем одну итерацию */
  qword0 = qword_ptr[0];
  qword1 = qword_ptr[1];
  qword_ptr += 2;
  if (__CMP_PRED (qword0) || __CMP_PRED (qword1) || (const char *) qword_ptr >= end_ptr)
    goto end;

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
  /* Which of the bytes was the zero? */
end:
  FIND_ZERO (qword0, qword1);
  tail = __builtin_ctz (mask);
  tail += (const char *) (qword_ptr - 2) - str;

#endif /* __iset__ > 5 */

  return tail > maxlen ? maxlen : tail;
}

#ifndef STRNLEN
libc_hidden_def (__strnlen)
weak_alias (__strnlen, strnlen)
#endif
libc_hidden_def (strnlen)
