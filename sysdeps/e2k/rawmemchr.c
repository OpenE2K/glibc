/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: rawmemchr.c 2068 2014-04-23 11:56:36Z vlog $
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#undef __ptr_t
#define __ptr_t void *

#if defined (_LIBC)
# include <string.h>
# include <memcopy.h>
# include <stdlib.h>
#endif

#include <e2kintrin.h>

#undef memchr

#ifndef RAWMEMCHR
# define RAWMEMCHR __rawmemchr
#endif

/* Find the first occurrence of C in S.  */
__ptr_t
RAWMEMCHR (s, c_in)
     const __ptr_t s;
     int c_in;
{
  const char *str = (const char *) s;
  unsigned long long charmask;

  /* Set up a longword, each of whose bytes is C.  */
  charmask = __builtin_e2k_pshufb (c_in, c_in, 0);

#if __iset__ <= 4

  const unsigned long long *longword_ptr;
  unsigned long long longword, mask, mask0, mask1, mask2, mask3;
  size_t tail, align;

  align = E2K_BYTES_FROM_ALIGN (str, 8);
  longword_ptr = E2K_ALIGN_PTR_BACK (str, 8);

  /* first longword */
  longword = longword_ptr[0];
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  mask >>= align * 8;

  if (mask != 0) {
    /* Which of the bytes was the zero? */
    __CTZB (tail, mask);
    return (__ptr_t) (str + tail);
  }

  /* next 3 longword */
  longword = longword_ptr[1];
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  if (mask != 0) {
    __CTZB (tail, mask);
    return (__ptr_t) ((const char *) (longword_ptr + 1) + tail);
  }

  longword = longword_ptr[2];
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  if (mask != 0) {
    __CTZB (tail, mask);
    return (__ptr_t) ((const char *) (longword_ptr + 2) + tail);
  }

  longword = longword_ptr[3];
  mask = __builtin_e2k_pcmpeqb (longword, charmask);
  if (mask != 0) {
    __CTZB (tail, mask);
    return (__ptr_t) ((const char *) (longword_ptr + 3) + tail);
  }

  longword_ptr = E2K_ALIGN_PTR_BACK (longword_ptr + 4, 32);

  /* We will test a 32 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (8)
  for (;;) {
    mask0 = __builtin_e2k_pcmpeqb (longword_ptr[0], charmask);
    mask1 = __builtin_e2k_pcmpeqb (longword_ptr[1], charmask);
    mask2 = __builtin_e2k_pcmpeqb (longword_ptr[2], charmask);
    mask3 = __builtin_e2k_pcmpeqb (longword_ptr[3], charmask);
    longword_ptr += 4;
    if ((mask0 | mask1 | mask2 | mask3) != 0) break;
  }
  /* Which of the bytes was the zero? */
  if (mask0 != 0) {
    __CTZB (tail, mask0);
    return (__ptr_t) ((const char *) (longword_ptr - 4) + tail);
  }
  if (mask1 != 0) {
    __CTZB (tail, mask1);
    return (__ptr_t) ((const char *) (longword_ptr - 3) + tail);
  }
  if (mask2 != 0) {
    __CTZB (tail, mask2);
    return (__ptr_t) ((const char *) (longword_ptr - 2) + tail);
  }
  __CTZB (tail, mask3);
  return (__ptr_t) ((const char *) (longword_ptr - 1) + tail);



#elif __iset__ <= 5

#define __CMP(mask, x) /* bytes with 'c_in' -> bit mask */                    \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))

  const __v2di *qword_ptr;
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  __v2di qword, qword0, qword1, qword2, qword3;
  unsigned int align, mask0, mask1, mask2, mask3;

  align = E2K_BYTES_FROM_ALIGN (str, 16);
  qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);

  /* first qword */
  qword = qword_ptr[0];
  __CMP (mask0, qword);
  mask0 >>= align; /* обнуляем каждый бит до начала строки */

  if (mask0 != 0) {
    /* Which of the bytes was C? */
    return (__ptr_t) (str + __builtin_ctz (mask0));
  }
  /* next 3 qwords */
  __CMP (mask1, qword_ptr[1]);
  if (mask1 != 0)
    return (__ptr_t) ((const char *) (qword_ptr + 1) + __builtin_ctz (mask1));

  __CMP (mask2, qword_ptr[2]);
  if (mask2 != 0)
    return (__ptr_t) ((const char *) (qword_ptr + 2) + __builtin_ctz (mask2));

  __CMP (mask3, qword_ptr[3]);
  if (mask3 != 0)
    return (__ptr_t) ((const char *) (qword_ptr + 3) + __builtin_ctz (mask3));

  qword_ptr = E2K_ALIGN_PTR_BACK (qword_ptr + 4, 64);

  /* We will test a 64 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 3 такта */
#pragma loop count (8)
  for (;;) {
    qword0 = __builtin_e2k_qpcmpeqb (qword_ptr[0], qcharmask);
    qword1 = __builtin_e2k_qpcmpeqb (qword_ptr[1], qcharmask);
    qword2 = __builtin_e2k_qpcmpeqb (qword_ptr[2], qcharmask);
    qword3 = __builtin_e2k_qpcmpeqb (qword_ptr[3], qcharmask);
    qword = __builtin_e2k_qpor (qword0, qword1);
    qword = __builtin_e2k_qplog (0xfe, qword, qword2, qword3); /* s1 | s2 | s3 */
    qword_ptr += 4;
    if (__builtin_e2k_qpsgn2mskb (qword) != 0) break;
  }
  /* Which of the bytes was C? */
  mask0 = __builtin_e2k_qpsgn2mskb (qword0);
  if (mask0 != 0)
    return (__ptr_t) ((const char *) (qword_ptr - 4) + __builtin_ctz (mask0));

  mask1 = __builtin_e2k_qpsgn2mskb (qword1);
  if (mask1 != 0)
    return (__ptr_t) ((const char *) (qword_ptr - 3) + __builtin_ctz (mask1));

  mask2 = __builtin_e2k_qpsgn2mskb (qword2);
  if (mask2 != 0)
    return (__ptr_t) ((const char *) (qword_ptr - 2) + __builtin_ctz (mask2));

  mask3 = __builtin_e2k_qpsgn2mskb (qword3);
  return (__ptr_t) ((const char *) (qword_ptr - 1) + __builtin_ctz (mask3));


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
  size_t tail;
  unsigned int mask;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4080, 0)) { /* closely to page border */
    /* Offsets 4081-4095 will be aligned into 4064 thus fit into page */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 32);
    FIND_CHAR (qword_ptr[0], qword_ptr[1]);
    mask >>= (str - (const char *) qword_ptr);

    if (mask != 0)
      return (__ptr_t) (str + __builtin_ctz (mask));
    qword_ptr += 2;
  }
  else {
    /* first qword load is unaligned */
    qword = ((__v2di *) str)[0];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was C? */
      __CMP (mask, qword);
      return (__ptr_t) (str + __builtin_ctz (mask));
    }
    /* next qword load is aligned */
    qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);
    qword = qword_ptr[1];

    if (__CMP_PRED (qword)) {
      /* Which of the bytes was C? */
      __CMP (mask, qword);
      tail = __builtin_ctz (mask);
      return (__ptr_t) ((const char *) (qword_ptr + 1) + tail);
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
  /* Which of the bytes was C? */
  FIND_CHAR (qword0, qword1);
  tail = __builtin_ctz (mask);
  return (__ptr_t) ((const char *) (qword_ptr - 2) + tail);

#endif /* __iset__ > 5 */

}

libc_hidden_def (__rawmemchr)
weak_alias (__rawmemchr, rawmemchr)
