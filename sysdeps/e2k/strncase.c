/* Copyright (c) 2015-2021 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strncase.c 2101 2014-05-13 11:24:32Z vlog $
 */

/* Compare at most N characters of two strings without taking care for
   the case.
   Copyright (C) 1992-2014 Free Software Foundation, Inc.
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <ctype.h>
#include <e2kintrin.h>

#ifndef weak_alias
# define __strncasecmp strncasecmp
# define TOLOWER(Ch) tolower (Ch)
#else
# include <locale/localeinfo.h>
# ifdef USE_IN_EXTENDED_LOCALE_MODEL
#  define __strncasecmp __strncasecmp_l
# endif
# define TOLOWER(Ch) __tolower_l ((Ch), loc)
#endif

#ifdef USE_IN_EXTENDED_LOCALE_MODEL
# define LOCALE_PARAM , loc
# define LOCALE_PARAM_DECL __locale_t loc;
#else
# define LOCALE_PARAM
# define LOCALE_PARAM_DECL
#endif

/* Type to use for aligned memory operations.
   This should normally be the biggest type supported by a single load
   and store.  Must be an unsigned type.  */
# define op_t   unsigned long long
# define OPSIZ  (sizeof(op_t))

/* Threshold value for when to enter the unrolled loops.  */
# define OP_T_THRES     16

# define CMP \
  ch1 = (a0      ) & 0xff; ch2 = (b0      ) & 0xff; result  = TOLOWER (ch1) ^ TOLOWER (ch2); \
  ch1 = (a0 >>  8) & 0xff; ch2 = (b0 >>  8) & 0xff; result |= TOLOWER (ch1) ^ TOLOWER (ch2); \
  ch1 = (a0 >> 16) & 0xff; ch2 = (b0 >> 16) & 0xff; result |= TOLOWER (ch1) ^ TOLOWER (ch2); \
  ch1 = (a0 >> 24) & 0xff; ch2 = (b0 >> 24) & 0xff; result |= TOLOWER (ch1) ^ TOLOWER (ch2); \
  ch1 = (a0 >> 32) & 0xff; ch2 = (b0 >> 32) & 0xff; result |= TOLOWER (ch1) ^ TOLOWER (ch2); \
  ch1 = (a0 >> 40) & 0xff; ch2 = (b0 >> 40) & 0xff; result |= TOLOWER (ch1) ^ TOLOWER (ch2); \
  ch1 = (a0 >> 48) & 0xff; ch2 = (b0 >> 48) & 0xff; result |= TOLOWER (ch1) ^ TOLOWER (ch2); \
  ch1 = (a0 >> 56)       ; ch2 = (b0 >> 56)       ; result |= TOLOWER (ch1) ^ TOLOWER (ch2)

static int
__strncasecmp_nonascii (const char *s1, const char *s2, size_t n, __locale_t loc);


/* Compare no more than N characters of S1 and S2,
   ignoring case, returning less than, equal to or
   greater than zero if S1 is lexicographically less
   than, equal to or greater than S2.  */
int
__strncasecmp (s1, s2, n LOCALE_PARAM)
     const char *s1;
     const char *s2;
     size_t n;
     LOCALE_PARAM_DECL
{
  unsigned long int srcp1 = (long int) s1;
  unsigned long int srcp2 = (long int) s2;

  if (s1 == s2 || n == 0)
    return 0;

#if defined _LIBC && !defined USE_IN_EXTENDED_LOCALE_MODEL
  __locale_t loc = _NL_CURRENT_LOCALE;
  op_t align1, align2;
  int res1, res2, tail;
  unsigned char ch1, ch2;

  if (n <= OP_T_THRES) { /* малые длины */
    /* There are just a few bytes to compare.  Use byte memory operations.  */
#pragma noprefetch
#pragma loop count (8)
    while (n > 0) {
      ch1 = *((unsigned char *) srcp1);
      ch2 = *((unsigned char *) srcp2);
      res1 = TOLOWER (ch1);
      res2 = TOLOWER (ch2);
      if (ch1 == '\0' || res1 != res2)
        break;
      n--;
      srcp1++;
      srcp2++;
    }
    return res1 - res2;
  }

  if (loc->__locales[LC_CTYPE]->values[_NL_ITEM_INDEX(_NL_CTYPE_NONASCII_CASE)].word & 1)
    /* используется локаль с таблицей, не соответствующей ascii кодам символов */
    return __strncasecmp_nonascii (s1, s2, n, loc);

  /* чисто strncmp c добавлением __TOLOWER */

#if __iset__ <= 4

#define __TOLOWER(x) {                                         \
    op_t m = __builtin_e2k_pcmpgtb (x, 0x4040404040404040LL) & \
             __builtin_e2k_pcmpgtb (0x5b5b5b5b5b5b5b5bLL, x);  \
    x |= m & 0x2020202020202020LL;                             \
}

  op_t a0, a00, a01, b0, spec, mask;
  unsigned long int end_ptr = (srcp2 + n - 1) & ~7;

  /* длины больше порога */
  if (__builtin_expect ((end_ptr < srcp2), 0))
    end_ptr = ~0UL << 3;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 8);
  mask = ((-1LL) << (align2 * 8));
  tail = ((srcp2 + n - 1) & 7) * 8;

  if (((srcp1 ^ srcp2) & 7) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~7;
    srcp2 &= ~7;
    a0 = ((op_t *) srcp1)[0] | ~mask; /* байты до начала строки заполняем 0xff */
    b0 = ((op_t *) srcp2)[0] | ~mask;
    __TOLOWER (a0);
    __TOLOWER (b0);
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    if (!(mask == 0 && a0 == b0)) goto m_last;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 3 такта */
#pragma loop count (100)
    for (;;) {
      a0 = ((op_t *) srcp1)[0];
      b0 = ((op_t *) srcp2)[0];
      mask = __builtin_e2k_pcmpeqb (b0, 0);
      __TOLOWER (a0);
      __TOLOWER (b0);
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      if (!(mask == 0 && a0 == b0 && srcp2 <= end_ptr)) break;
    }
  }
  else { /* разное выравнивание строк */

    srcp1 -= align2;
    align1 = E2K_BYTES_FROM_ALIGN (srcp1, 8);
    srcp1 &= ~7;
    srcp2 &= ~7;
    E2K_PREPARE_ALIGN (align1, spec);
    a00 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 0); /* чтобы не залезть за левую границу */
    a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8); /* чтобы не залезть за правую границу */
    E2K_ALIGN_DATA (a00, a01, a0, spec);
    b0 = ((op_t *) srcp2)[0];
    a0 |= ~mask; /* байты до начала строки заполняем 0xff */
    b0 |= ~mask;
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    __TOLOWER (a0);
    __TOLOWER (b0);
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    if (!(mask == 0 && a0 == b0)) goto m_last;

    a00 = a01;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 3 такта */
#pragma loop count (100)
    for (;;) {
      a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8);
      E2K_ALIGN_DATA (a00, a01, a0, spec);
      b0 = ((op_t *) srcp2)[0];
      mask = __builtin_e2k_pcmpeqb (b0, 0);
      __TOLOWER (a0);
      __TOLOWER (b0);
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      a00 = a01;
      if (!(mask == 0 && a0 == b0 && srcp2 <= end_ptr)) break;
    }
  }
m_last:
  if (srcp2 > end_ptr) { /* вышли по последнему qword */
    mask |= (-1LL) << tail; /* вставляем 1 в каждый бит после хвоста строки */
  }
  spec = __builtin_ctzll (mask | ~__builtin_e2k_pcmpeqb (a0, b0));
  align1 = (a0 >> spec) & 0xff;
  align2 = (b0 >> spec) & 0xff;
  return align1 - align2;


#elif __iset__ <=  5

#define __CMP(mask, x, y) /* bytes compare -> bit mask */             \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, y))
#define __CMP0(mask, x)   /* zero bytes -> bit mask */                \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __TOLOWER(x) {                                                   \
    __v2di m = __builtin_e2k_qpand (__builtin_e2k_qpcmpgtb (x, quclow),  \
                                    __builtin_e2k_qpcmpgtb (quchigh, x));\
    x = __builtin_e2k_qplog (0xf8, x, m, qucdiff); /* s1 | (s2 & s3) */  \
}

  __v2di a0, a00, a01, b0, spec;
  const __v2di qzero = (__v2di) {0, 0};
  const __v2di quclow = (__v2di) {0x4040404040404040LL, 0x4040404040404040LL};
  const __v2di quchigh = (__v2di) {0x5b5b5b5b5b5b5b5bLL, 0x5b5b5b5b5b5b5b5bLL};
  const __v2di qucdiff = (__v2di) {0x2020202020202020LL, 0x2020202020202020LL};
  unsigned long int end_ptr = (srcp2 + n - 1) & ~15;
  unsigned int mask, mask0;

  /* длины больше порога */
  if ((srcp2 + n - 1) < srcp2)
    end_ptr = ~0UL << 4;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 16);
  tail = (srcp2 + n - 1) & 15;

  if (((srcp1 ^ srcp2) & 15) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~15;
    srcp2 &= ~15;
    a0 = ((__v2di *) srcp1)[0];
    b0 = ((__v2di *) srcp2)[0];
    __CMP0 (mask0, b0);
    __TOLOWER (a0);
    __TOLOWER (b0);
    srcp1 += 16;
    srcp2 += 16;
    __CMP (mask, a0, b0);
    mask |= ~((-1) << align2); /* вставляем 1 в каждый бит до начала строки */
    mask0 &= ((-1) << align2); /* обнуляем каждый бит до начала строки */

    if (mask == 0xffff && mask0 == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 4 такта */
#pragma loop count (100)
      for (;;) {
        a0 = ((__v2di *) srcp1)[0];
        b0 = ((__v2di *) srcp2)[0];
        __CMP0 (mask0, b0);
        __TOLOWER (a0);
        __TOLOWER (b0);
        srcp1 += 16;
        srcp2 += 16;
        __CMP (mask, a0, b0);
        if (!(mask == 0xffff && mask0 == 0 && srcp2 <= end_ptr)) break;
      }
    }
  }
  else { /* разное выравнивание строк */

    srcp1 -= align2;
    align1 = E2K_BYTES_FROM_ALIGN (srcp1, 16);
    srcp1 &= ~15;
    srcp2 &= ~15;
    E2K_PREPARE_ALIGN128 (align1, spec);
    a00 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 0); /* чтобы не залезть за левую границу */
    a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16); /* чтобы не залезть за правую границу */
    E2K_ALIGN_DATA128 (a00, a01, a0, spec);
    b0 = ((__v2di *) srcp2)[0];
    __CMP0 (mask0, b0);
    __TOLOWER (a0);
    __TOLOWER (b0);
    srcp1 += 16;
    srcp2 += 16;
    __CMP (mask, a0, b0);
    mask |= ~((-1) << align2); /* вставляем 1 в каждый бит до начала строки */
    mask0 &= ((-1) << align2); /* обнуляем каждый бит до начала строки */

    if (mask == 0xffff && mask0 == 0) {

      a00 = a01;

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 4 такта */
#pragma loop count (100)
      for (;;) {
        a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
        E2K_ALIGN_DATA128 (a00, a01, a0, spec);
        b0 = ((__v2di *) srcp2)[0];
        __CMP0 (mask0, b0);
        __TOLOWER (a0);
        __TOLOWER (b0);
        srcp1 += 16;
        srcp2 += 16;
        a00 = a01;
        __CMP (mask, a0, b0);
        if (!(mask == 0xffff && mask0 == 0 && srcp2 <= end_ptr)) break;
      }
    }
  }
  mask = mask0 | ~mask;
  if (srcp2 > end_ptr) { /* вышли по последнему qword */
    mask |= ((-1) << tail); /* вставляем 1 в каждый бит после хвоста строки */
  }
  mask = __builtin_ctz (mask);
  __v2di aa = a0;
  __v2di bb = b0;
  align1 = __builtin_e2k_pshufb (aa[1], aa[0], mask) & 0xff;
  align2 = __builtin_e2k_pshufb (bb[1], bb[0], mask) & 0xff;
  return align1 - align2;


#else /* __iset__ > 5 */

#define __CMP(mask, x, y) /* bytes compare -> bit mask */             \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, y))
#define __CMP0(mask, x)   /* zero bytes -> bit mask */                \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __CMP_PRED(x, y)  /* bytes compare -> predicate */            \
    __builtin_e2k_qpcmpeqbap (x, y)
#define __CMP0_PRED(x)    /* zero bytes -> predicate */               \
    __builtin_e2k_qpcmpeqbop (x, qzero)
#define __TOLOWER(x) {                                                   \
    __v2di m = __builtin_e2k_qpand (__builtin_e2k_qpcmpgtb (x, quclow),  \
                                    __builtin_e2k_qpcmpgtb (quchigh, x));\
    x = __builtin_e2k_qplog (0xf8, x, m, qucdiff); /* s1 | (s2 & s3) */  \
}

  __v2di a0, a00, a01, b0, spec, qpmask;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  const __v2di quclow = __builtin_e2k_qppackdl (0x4040404040404040LL, 0x4040404040404040LL);
  const __v2di quchigh = __builtin_e2k_qppackdl (0x5b5b5b5b5b5b5b5bLL, 0x5b5b5b5b5b5b5b5bLL);
  const __v2di qucdiff = __builtin_e2k_qppackdl (0x2020202020202020LL, 0x2020202020202020LL);
  unsigned long int end_ptr = (srcp2 + n - 1) & ~15;
  unsigned int mask, mask0;

  /* длины больше порога */
  if (end_ptr < srcp2)
    end_ptr = ~0UL << 4;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 16);
  tail = (srcp2 + n - 1) & 15;

  if (((srcp1 ^ srcp2) & 15) == 0) { /* обе строки выровнены одинаково */
    if (__builtin_expect ((E2K_BYTES_FROM_ALIGN (srcp1, 4096) > 4080) ||
                          (E2K_BYTES_FROM_ALIGN (srcp2, 4096) > 4080), 0)) { /* closely to page border */
      /* Offsets 4081-4095 will be shifted back to the aligned address thus fit into page */
      srcp1 &= ~15;
      srcp2 &= ~15;
      /* first qword loads are aligned */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      __TOLOWER (a0);
      __TOLOWER (b0);
      srcp1 += 16;
      srcp2 += 16;
      qpmask = __builtin_e2k_qpmsk2sgnb (qzero, (1 << align2) - 1);
      qpmask = __builtin_e2k_qpcmpgtb (qzero, qpmask);
      a0 = __builtin_e2k_qpor (a0, qpmask); /* вставляем 0xff в каждый байт до начала строки */
      b0 = __builtin_e2k_qpor (b0, qpmask);
    }
    else {
      /* first qword loads are unaligned */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      __TOLOWER (a0);
      __TOLOWER (b0);
      srcp1 += 16 - align2;
      srcp2 += 16 - align2;
    }
    if (__CMP_PRED (a0, b0) && __CMP0_PRED (b0) == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 3 такта */
#pragma loop count (100)
      for (;;) {
        a0 = ((__v2di *) srcp1)[0];
        b0 = ((__v2di *) srcp2)[0];
        __TOLOWER (a0);
        __TOLOWER (b0);
        srcp1 += 16;
        srcp2 += 16;
        if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0) || srcp2 > end_ptr) break;
      }
    }
  }
  else { /* разное выравнивание строк */

    align1 = E2K_BYTES_FROM_ALIGN (srcp1 - align2, 16);
    E2K_PREPARE_ALIGN128 (align1, spec);

    if (__builtin_expect ((E2K_BYTES_FROM_ALIGN (srcp1, 4096) > 4080) ||
                          (E2K_BYTES_FROM_ALIGN (srcp2, 4096) > 4080), 0)) { /* closely to page border */
      /* Offsets 4081-4095 will be shifted back to the aligned address thus fit into page */
      srcp1 -= align2;
      srcp1 &= ~15;
      srcp2 &= ~15;
      a00 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 0); /* чтобы не залезть за левую границу */
      a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
      E2K_ALIGN_DATA128 (a00, a01, a0, spec);
      b0 = ((__v2di *) srcp2)[0];
      __TOLOWER (a0);
      __TOLOWER (b0);
      qpmask = __builtin_e2k_qpmsk2sgnb (qzero, (1 << align2) - 1);
      qpmask = __builtin_e2k_qpcmpgtb (qzero, qpmask);
      a0 = __builtin_e2k_qpor (a0, qpmask); /* вставляем 0xff в каждый байт до начала строки */
      b0 = __builtin_e2k_qpor (b0, qpmask);
      a00 = a01;
      srcp1 += 16;
      srcp2 += 16;
    }
    else {
      /* first qword loads are unaligned */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      __TOLOWER (a0);
      __TOLOWER (b0);
      srcp1 += 16 - align2;
      srcp2 += 16 - align2;
      srcp1 &= ~15;
      /* next qword loads are aligned */
      a00 = ((__v2di *) srcp1)[0];
    }
    if (__CMP_PRED (a0, b0) && __CMP0_PRED (b0) == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 3 такта */
#pragma loop count (100)
      for (;;) {
//        a01 = ((__v2di *) srcp1)[1];
        a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
        E2K_ALIGN_DATA128 (a00, a01, a0, spec);
        b0 = ((__v2di *) srcp2)[0];
        __TOLOWER (a0);
        __TOLOWER (b0);
        srcp1 += 16;
        srcp2 += 16;
        a00 = a01;
        if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0) || srcp2 > end_ptr) break;
      }
    }
  }
  __CMP0 (mask0, b0);
  __CMP (mask, a0, b0);
  mask = mask0 | ~mask;
  if (srcp2 > end_ptr) { /* вышли по последнему qword */
    mask |= ((-1) << tail); /* вставляем 1 в каждый бит после хвоста строки */
  }
  mask = __builtin_ctz (mask);
  __v2di aa = a0;
  __v2di bb = b0;
  align1 = __builtin_e2k_pandd (__builtin_e2k_pshufb (aa[1], aa[0], mask), 0xff);
  align2 = __builtin_e2k_pandd (__builtin_e2k_pshufb (bb[1], bb[0], mask), 0xff);
  return __builtin_e2k_psubw (align1, align2);

#endif /* __iset__ > 5 */

}


/* медленный вариант работы с произвольной таблицей */
static int
__strncasecmp_nonascii (const char *s1, const char *s2, size_t n, __locale_t loc)
{
  unsigned long int srcp1 = (unsigned long int) s1;
  unsigned long int srcp2 = (unsigned long int) s2;

#endif /* defined _LIBC && !defined USE_IN_EXTENDED_LOCALE_MODEL */

  op_t a0, a00, a01, b0, align1, align2, spec, mask;
  unsigned long int end_ptr = (srcp2 + n - 1) & ~7;
  int i, result, res1, res2, tail;
  unsigned char ch1, ch2;

  /* длины больше порога */
  if ((srcp2 + n) < srcp2)
    end_ptr = ~0UL << 3;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 8);
  mask = ((-1LL) << (align2 * 8));
  tail = (srcp2 + n - 1) & 7;

  if (((srcp1 ^ srcp2) & 7) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~7;
    srcp2 &= ~7;
    a0 = ((op_t *) srcp1)[0] | ~mask; /* байты до начала строки заполняем 0xff */
    b0 = ((op_t *) srcp2)[0] | ~mask;
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    CMP;
    if (!(mask == 0 && result == 0)) goto m_last1;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 8 тактов (9 - в режиме 64) */
#pragma loop count (100)
    for (;;) {
      a0 = ((op_t *) srcp1)[0];
      b0 = ((op_t *) srcp2)[0];
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      mask = __builtin_e2k_pcmpeqb (b0, 0);
      CMP;
      if (!(mask == 0 && result == 0 && srcp2 <= end_ptr)) break;
    }
  }
  else { /* разное выравнивание строк */

    srcp1 -= align2;
    align1 = E2K_BYTES_FROM_ALIGN (srcp1, 8);
    srcp1 &= ~7;
    srcp2 &= ~7;
    E2K_PREPARE_ALIGN (align1, spec);
    a00 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 0); /* чтобы не залезть за левую границу */
    a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8); /* чтобы не залезть за правую границу */
    E2K_ALIGN_DATA (a00, a01, a0, spec);
    b0 = ((op_t *) srcp2)[0];
    a0 |= ~mask; /* байты до начала строки заполняем 0xff */
    b0 |= ~mask;
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    CMP;
    a00 = a01;

    if (!(mask == 0 && result == 0)) goto m_last1;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 9 тактов (10 - в режиме 64) */
#pragma loop count (100)
    for (;;) {
      a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8);
      E2K_ALIGN_DATA (a00, a01, a0, spec);
      b0 = ((op_t *) srcp2)[0];
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      a00 = a01;
      mask = __builtin_e2k_pcmpeqb (b0, 0);
      CMP;
      if (!(mask == 0 && result == 0 && srcp2 <= end_ptr)) break;
    }
  }

m_last1:
  if (srcp2 <= end_ptr) { /* вышли не по последнему qword */
    if (mask != 0) { /* нашли конец строки */
      __CTZB (tail, mask);
    }
    else tail = 7;
  }

#pragma loop count (3)
  for (i = 0; i <= tail; i++) { /* обработка до конца строки или 8 байт */
    ch1 = a0 & 0xff;
    ch2 = b0 & 0xff;
    res1 = TOLOWER (ch1);
    res2 = TOLOWER (ch2);
    a0 >>= 8;
    b0 >>= 8;
    if (ch1 == '\0' || res1 != res2)
      return res1 - res2;
  }
  return 0;
}

#ifndef __strncasecmp
weak_alias (__strncasecmp, strncasecmp)
#endif
