/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: memcmp.c 2218 2014-06-16 13:57:59Z vlog $
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#undef  __ptr_t
#define __ptr_t void *

#if defined HAVE_STRING_H || defined _LIBC
# include <string.h>
#endif

#undef memcmp

#ifndef MEMCMP
# define MEMCMP memcmp
#endif

#ifdef _LIBC

# include <memcopy.h>

#else   /* Not in the GNU C library.  */

# include <sys/types.h>

/* Type to use for aligned memory operations.
   This should normally be the biggest type supported by a single load
   and store.  Must be an unsigned type.  */
# define op_t   unsigned long long
# define OPSIZ  (sizeof(op_t))

/* Type to use for unaligned operations.  */
typedef unsigned char byte;

#endif  /* In the GNU C library.  */

#include <e2kintrin.h>

//#undef E2K_PREPARE_ALIGN
//#undef E2K_ALIGN_DATA
///* вариант на операциях insfd/scrd для данных формата 64 */
//#define E2K_PREPARE_ALIGN(align, spec) spec = align << 9
//#define E2K_ALIGN_DATA(src1, src2, dst, spec)             \
//    dst = __builtin_e2k_scrd (__builtin_e2k_insfd (src1, spec, src2), spec >> 6)

int
MEMCMP (s1, s2, len)
     const __ptr_t s1;
     const __ptr_t s2;
     size_t len;
{
  op_t align1, align2;
  unsigned long int srcp1 = (unsigned long int) s1;
  unsigned long int srcp2 = (unsigned long int) s2;
  long int tail;

  if (len <= 16) { /* малые длины */
        /* There are just a few bytes to compare.  Use byte memory operations.  */
    byte *ss1 = (byte *) s1;
    byte *ss2 = (byte *) s2;
    int i, res = 0;

#pragma noprefetch
#pragma loop count (8)
    for (i = 0; i < (int) len; i++) {
      res = ss1[i] - ss2[i];
      if (res != 0) break;
    }
    return res;
  }

#if __iset__ <= 4

  op_t a0, a00, a01, b0, spec, mask;
  unsigned long int end_ptr = (srcp2 + len) & ~7;

  /* длины больше порога */
  if (__builtin_expect (end_ptr < srcp2, 0))
    end_ptr = ~0UL << 3;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 8);
  mask = ((-1LL) << (align2 * 8));
  len -= 8 - align2;
  tail = (len & 7) * 8;

  if (((srcp1 ^ srcp2) & 7) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~7;
    srcp2 &= ~7;
    a0 = ((op_t *) srcp1)[0] | ~mask; /* байты до начала строки заполняем 0xff */
    b0 = ((op_t *) srcp2)[0] | ~mask;
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
    for (;;) {
      if (!(a0 == b0 && srcp2 < end_ptr)) break;
      a0 = ((op_t *) srcp1)[0];
      b0 = ((op_t *) srcp2)[0];
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
    }
    if (a0 != b0) goto m_last; /* вышли из цикла по не сравнению */

    if (tail) { /* байты после конца строки заполняем 0xff */
      mask = (-1LL) << tail;
      a0 = ((op_t *) srcp1)[0] | mask;
      b0 = ((op_t *) srcp2)[0] | mask;
      if (a0 != b0) goto m_last;
    }
  }
  else { /* разное выравнивание строк */

    srcp1 -= align2;
    align1 = E2K_BYTES_FROM_ALIGN (srcp1, 8);
    srcp1 &= ~7;
    srcp2 &= ~7;
    E2K_PREPARE_ALIGN (align1, spec);
    a00 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 0); /* чтобы не залезть за левую границу */
    a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8);
    E2K_ALIGN_DATA (a00, a01, a0, spec);
    b0 = ((op_t *) srcp2)[0];
    a0 |= ~mask; /* байты до начала строки заполняем 0xff */
    b0 |= ~mask;
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    if (a0 != b0) goto m_last;

    a00 = a01;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
    for (;;) {
      a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8);
      E2K_ALIGN_DATA (a00, a01, a0, spec);
      b0 = ((op_t *) srcp2)[0];
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      a00 = a01;
      if (!(a0 == b0 && srcp2 < end_ptr)) break;
    }
    if (a0 != b0) goto m_last; /* вышли из цикла по не сравнению */

    if (tail) { /* байты после конца строки заполняем 0xff */
      mask = (-1LL) << tail;
      a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8);
      E2K_ALIGN_DATA (a00, a01, a0, spec);
      b0 = ((op_t *) srcp2)[0];
      a0 |= mask;
      b0 |= mask;
      if (a0 != b0) {
m_last:
        a0 = __builtin_bswap64 (a0); /* разворот байт */
        b0 = __builtin_bswap64 (b0); /* разворот байт */
        return a0 > b0 ? 1 : -1;
      }
    }
  }


#elif __iset__ <= 5

#define __CMP(mask, x, y) /* bytes compare -> bit mask */             \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, y))

  __v2di a0, a00, a01, b0, spec;
  unsigned long int end_ptr = (srcp2 + len) & ~15;
  unsigned int mask;

  /* длины больше порога */
  if ((srcp2 + len) < srcp2)
    end_ptr = ~0UL << 4;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 16);
  len -= 16 - align2;
  tail = len & 15;

  if (((srcp1 ^ srcp2) & 15) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~15;
    srcp2 &= ~15;
    a0 = ((__v2di *) srcp1)[0];
    b0 = ((__v2di *) srcp2)[0];
    __CMP (mask, a0, b0);
    mask |= ~((-1) << align2); /* вставляем 1 в каждый бит до начала строки */
    srcp1 += 16;
    srcp2 += 16;
    if (mask != 0xffff) goto m_last;

    if (srcp2 < end_ptr) {
      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
      for (;;) {
        a0 = ((__v2di *) srcp1)[0];
        b0 = ((__v2di *) srcp2)[0];
        __CMP (mask, a0, b0);
        srcp1 += 16;
        srcp2 += 16;
        if (!(mask == 0xffff && srcp2 < end_ptr)) break;
      }
      if (mask != 0xffff) goto m_last; /* вышли из цикла по не сравнению */
    }
    if (tail) { /* биты в маске после конца строки заполняем 1 */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      __CMP (mask, a0, b0);
      mask |= ((-1) << tail) & 0xffff;
      if (mask != 0xffff) goto m_last;
    }
  }
  else { /* разное выравнивание строк */

    srcp1 -= align2;
    align1 = E2K_BYTES_FROM_ALIGN (srcp1, 16);
    srcp1 &= ~15;
    srcp2 &= ~15;
    E2K_PREPARE_ALIGN128 (align1, spec);
    a00 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 0); /* чтобы не залезть за левую границу */
    a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
    E2K_ALIGN_DATA128 (a00, a01, a0, spec);
    b0 = ((__v2di *) srcp2)[0];
    __CMP (mask, a0, b0);
    mask |= ~((-1) << align2); /* вставляем 1 в каждый бит до начала строки */
    srcp1 += 16;
    srcp2 += 16;
    if (mask != 0xffff) goto m_last;

    a00 = a01;

    if (srcp2 < end_ptr) {
      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
#pragma unroll (1)
      for (;;) {
        a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
        E2K_ALIGN_DATA128 (a00, a01, a0, spec);
        b0 = ((__v2di *) srcp2)[0];
        srcp1 += 16;
        srcp2 += 16;
        a00 = a01;
        __CMP (mask, a0, b0);
        if (!(mask == 0xffff && srcp2 < end_ptr)) break;
      }
      if (mask != 0xffff) goto m_last; /* вышли из цикла по не сравнению */
    }
    if (tail) { /* биты в маске после конца строки заполняем 1 */
      a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
      E2K_ALIGN_DATA128 (a00, a01, a0, spec);
      b0 = ((__v2di *) srcp2)[0];
      __CMP (mask, a0, b0);
      mask |= ((-1) << tail) & 0xffff;
      if (mask != 0xffff) {
m_last:
        mask = __builtin_ctz (~mask);
        __v2di aa = a0;
        __v2di bb = b0;
        align1 = __builtin_e2k_pandd (__builtin_e2k_pshufb (aa[1], aa[0], mask), 0xff);
        align2 = __builtin_e2k_pandd (__builtin_e2k_pshufb (bb[1], bb[0], mask), 0xff);
        return __builtin_e2k_psubw (align1, align2);
      }
    }
  }


#else /* __iset__ > 5 */

#define __CMP(mask, x, y) /* bytes compare -> bit mask */             \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, y))
#define __CMP_PRED(x, y)  /* bytes compare -> predicate */            \
    __builtin_e2k_qpcmpeqbap (x, y)

  __v2di a0, b0;
  unsigned long int end_ptr = srcp2 + (len & ~15);
  unsigned int mask;

  /* длины больше порога */
  if ((srcp2 + len) < srcp2)
    end_ptr = ~0UL;

  tail = len & 15;

  /* откручиваем одну итерацию */
  a0 = ((__v2di *) srcp1)[0];
  b0 = ((__v2di *) srcp2)[0];
  srcp1 += 16;
  srcp2 += 16;
  __CMP (mask, a0, b0);
  if (__CMP_PRED (a0, b0) == 0) goto m_last;

  if (srcp2 < end_ptr) {
    /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
#pragma unroll (1)
    for (;;) {
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      srcp1 += 16;
      srcp2 += 16;
      if (!(__CMP_PRED (a0, b0) && srcp2 < end_ptr)) break;
    }
    __CMP (mask, a0, b0);
    if (mask != 0xffff) goto m_last; /* вышли из цикла по не сравнению */
  }
  if (tail) { /* биты в маске до начала хвоста строки заполняем 1 */
    a0 = ((__v2di *) (srcp1 - 16 + tail))[0];
    b0 = ((__v2di *) (srcp2 - 16 + tail))[0];
    __CMP (mask, a0, b0);
    mask |= 0xffff >> tail;
    if (mask != 0xffff) {
m_last:
      mask = __builtin_ctz (~mask);
      __v2di aa = a0;
      __v2di bb = b0;
      align1 = __builtin_e2k_pandd (__builtin_e2k_pshufb (aa[1], aa[0], mask), 0xff);
      align2 = __builtin_e2k_pandd (__builtin_e2k_pshufb (bb[1], bb[0], mask), 0xff);
      return __builtin_e2k_psubw (align1, align2);
    }
  }

#endif /* __iset__ > 5 */

  return 0;
}

libc_hidden_builtin_def(memcmp)
#ifdef weak_alias
# undef bcmp
weak_alias (memcmp, bcmp)
#endif
