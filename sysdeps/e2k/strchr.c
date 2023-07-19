/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strchr.c 2068 2014-04-23 11:56:36Z vlog $
 */

#include <string.h>
#include <memcopy.h>
#include <stdlib.h>
#include <e2kintrin.h>

#undef strchr

/* Find the first occurrence of C in S.  */
char *
strchr (str, c_in)
     const char *str;
     int c_in;
{
  /* Set up a longword, each of whose bytes is C.  */
  unsigned long long charmask = __builtin_e2k_pshufb (c_in, c_in, 0);

#if __iset__ <= 4

  const unsigned long long *longword_ptr;
  unsigned long long longword, mask, mask0;
  size_t align;

  align = E2K_BYTES_FROM_ALIGN (str, 8);
  longword_ptr = E2K_ALIGN_PTR_BACK (str, 8);

  longword = *longword_ptr++;
  mask0 = __builtin_e2k_pcmpeqb (longword, 0);
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  mask0 &= (-1LL) << (align * 8); /* обнуляем каждый байт до начала строки */
  mask &= (-1LL) << (align * 8);

  if ((mask0 | mask) != 0) {
    /* Which of the bytes was the zero? */
    __CTZB (mask, mask);
    __CTZB (mask0, mask0);
    return (mask0 < mask) ? NULL : (char *) (longword_ptr - 1) + mask;
  }

  /* откручиваем одну итерацию */
  longword = *longword_ptr++;
  mask0 = __builtin_e2k_pcmpeqb (longword, 0);
  mask = __builtin_e2k_pcmpeqb (longword, charmask);

  if ((mask0 | mask) == 0) {

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch
#pragma loop count (8) /* иначе применяется apb, 1 такт */
    for (;;) {
      longword = *longword_ptr++;
      mask0 = __builtin_e2k_pcmpeqb (longword, 0);
      mask = __builtin_e2k_pcmpeqb (longword, charmask);
      if ((mask0 | mask) != 0) break;
    }
  }
  /* Which of the bytes was the zero? */
  __CTZB (mask, mask);
  __CTZB (mask0, mask0);
  return (mask0 < mask) ? NULL : (char *) (longword_ptr - 1) + mask;

#elif __iset__ <= 5

#define __CMP(mask, x)  /* bytes with 'c_in' -> bit mask */               \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))
#define __CMP0(mask, x) /* zero bytes -> bit mask */                      \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))

  const __v2di *qword_ptr, qzero = __builtin_e2k_qppackdl (0, 0);
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  __v2di qword, qword0, qword1, qword2, qword3;
  unsigned int mask, mask0, mask1, mask2, align;

  align = E2K_BYTES_FROM_ALIGN (str, 32);
  qword_ptr = E2K_ALIGN_PTR_BACK (str, 32);

  /* first qword */
  qword = qword_ptr[0];
  __CMP (mask, qword);
  __CMP0 (mask0, qword);
  /* next qword */
  qword = qword_ptr[1];
  __CMP (mask1, qword);
  __CMP0 (mask2, qword);
  mask |= mask1 << 16;
  mask0 |= mask2 << 16;
  mask >>= align;  /* обнуляем каждый бит до начала строки */
  mask0 >>= align;

  if ((mask0 | mask) != 0) {
    /* Which of the bytes was the zero? */
    mask = __builtin_ctz (mask);
    mask0 = __builtin_ctz (mask0);
    return (mask0 < mask) ? NULL : (char *) str + mask;
  }
  qword_ptr += 2;

  /* We will test a 32 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
  for (;;) {
    qword0 = __builtin_e2k_qpcmpeqb (qword_ptr[0], qcharmask);
    qword1 = __builtin_e2k_qpcmpeqb (qword_ptr[1], qcharmask);
    qword2 = __builtin_e2k_qpcmpeqb (qword_ptr[0], qzero);
    qword3 = __builtin_e2k_qpcmpeqb (qword_ptr[1], qzero);
    qword = __builtin_e2k_qpor (qword0, qword1);
    qword = __builtin_e2k_qplog (0xfe, qword, qword2, qword3); /* s1 | s2 | s3 */
    qword_ptr += 2;
    if (__builtin_e2k_qpsgn2mskb (qword) != 0) break;
  }
  /* Which of the bytes was the zero? */
  mask = __builtin_e2k_qpsgn2mskb (qword0);
  mask0 = __builtin_e2k_qpsgn2mskb (qword2);
  if ((mask0 | mask) != 0) {
    mask = __builtin_ctz (mask);
    mask0 = __builtin_ctz (mask0);
    return (mask0 < mask) ? NULL : (char *) (qword_ptr - 2) + mask;
  }
  mask = __builtin_e2k_qpsgn2mskb (qword1);
  mask0 = __builtin_e2k_qpsgn2mskb (qword3);
  mask = __builtin_ctz (mask);
  mask0 = __builtin_ctz (mask0);
  return (mask0 < mask) ? NULL : (char *) (qword_ptr - 1) + mask;

#else /* __iset__ > 5 */

#define __CMP(mask, x)  /* bytes with 'c_in' -> bit mask */               \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))
#define __CMP0(mask, x) /* zero bytes -> bit mask */                      \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __CMP_PRED(x)   /* bytes with 'c_in' -> predicate */              \
    __builtin_e2k_qpcmpeqbop (x, qcharmask)
#define __CMP0_PRED(x)  /* zero bytes -> predicate */                     \
    __builtin_e2k_qpcmpeqbop (x, qzero)

  const __v2di *qword_ptr, qzero = (__v2di) {0, 0};
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  __v2di qword;
  unsigned int mask, mask0;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4080, 0)) { /* closely to page border */
    /* Offsets 4081-4095 will be aligned into 4080 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);
    qword = qword_ptr[0];
    __CMP (mask, qword);
    __CMP0 (mask0, qword);
    mask >>= (str - (const char *) qword_ptr);
    mask0 >>= (str - (const char *) qword_ptr);

    if ((mask0 | mask) != 0) {
      /* Which of the bytes was the zero? */
      mask = __builtin_ctz (mask);
      mask0 = __builtin_ctz (mask0);
      return (mask0 < mask) ? NULL : (char *) str + mask;
    }
    qword_ptr++;
  }
  else {
    /* first qword load is unaligned */
    qword = ((__v2di *) str)[0];

    if (__CMP_PRED (qword) || __CMP0_PRED (qword)) {
      /* Which of the bytes was the zero? */
      __CMP (mask, qword);
      __CMP0 (mask0, qword);
      mask = __builtin_ctz (mask);
      mask0 = __builtin_ctz (mask0);
      return (mask0 < mask) ? NULL : (char *) str + mask;
    }
    qword_ptr = E2K_ALIGN_PTR_BACK (str + 16, 16);
  }

  /* откручиваем одну итерацию */
  qword = *qword_ptr++;

  if (__CMP_PRED (qword) == 0 && __CMP0_PRED (qword) == 0) {

    /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
    for (;;) {
      qword = *qword_ptr++;
      if (__CMP_PRED (qword) || __CMP0_PRED (qword)) break;
    }
  }
  /* Which of the bytes was the zero? */
  __CMP (mask, qword);
  __CMP0 (mask0, qword);
  mask = __builtin_ctz (mask);
  mask0 = __builtin_ctz (mask0);
  return (mask0 < mask) ? NULL : (char *) (qword_ptr - 1) + mask;

#endif /* __iset__ > 5 */

}

#ifdef weak_alias
#undef index
weak_alias (strchr, index)
#endif
libc_hidden_builtin_def (strchr)
