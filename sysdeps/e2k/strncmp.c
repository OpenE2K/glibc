/* Copyright (c) 2015-2021 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strncmp.c 2218 2014-06-16 13:57:59Z vlog $
 */

#include <string.h>
#include <memcopy.h>
#include <e2kintrin.h>

#undef E2K_PREPARE_ALIGN
#undef E2K_ALIGN_DATA
/* вариант на операциях insfd/scrd для данных формата 64 */
#define E2K_PREPARE_ALIGN(align, spec) spec = align << 9
#define E2K_ALIGN_DATA(src1, src2, dst, spec)             \
    dst = __builtin_e2k_scrd (__builtin_e2k_insfd (src1, spec, src2), spec >> 6)

#undef strncmp

#ifndef STRNCMP
#define STRNCMP strncmp
#endif

/* Compare no more than N characters of S1 and S2,
   returning less than, equal to or greater than zero
   if S1 is lexicographically less than, equal to or
   greater than S2.  */

int
STRNCMP (const char *s1, const char *s2, size_t len)
{
  unsigned long int srcp1 = (unsigned long int) s1;
  unsigned long int srcp2 = (unsigned long int) s2;
  op_t align1, align2;
  long int tail;

  if (len <= 32) { /* малые длины */
    /* There are just a few bytes to compare.  Use byte memory operations.  */
    unsigned char * ss1 = (unsigned char *) s1;
    unsigned char * ss2 = (unsigned char *) s2;
    int i, res = 0;

#pragma noprefetch
#pragma loop count (20)
    for (i = 0; i < (int) len; i++) {
      res = ss1[i] - ss2[i];
      if (!(res == 0 && ss1[i] != '\0')) break;
    }
    return res;
  }

#if __iset__ <= 4

  op_t a0, a00, a01, b0, spec, mask;
  unsigned long int end_ptr = (srcp2 + len - 1) & ~7;

  /* длины больше порога */
  if (__builtin_expect ((end_ptr < srcp2), 0))
    end_ptr = ~0UL << 3;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 8);
  mask = ((-1LL) << (align2 * 8));
  tail = ((srcp2 + len - 1) & 7) * 8;

  if (((srcp1 ^ srcp2) & 7) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~7;
    srcp2 &= ~7;
    a0 = ((op_t *) srcp1)[0] | ~mask; /* байты до начала строки заполняем 0xff */
    b0 = ((op_t *) srcp2)[0] | ~mask;
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    if (!(mask == 0 && a0 == b0)) goto m_last;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
    for (;;) {
      a0 = ((op_t *) srcp1)[0];
      b0 = ((op_t *) srcp2)[0];
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      mask = __builtin_e2k_pcmpeqb (b0, 0);
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
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    a00 = a01;

    if (!(mask == 0 && a0 == b0)) goto m_last;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
    for (;;) {
//      a01 = ((op_t *) srcp1)[1];
      a01 = __builtin_e2k_ld_64s_cleartag ((op_t *) srcp1, 8);
      E2K_ALIGN_DATA (a00, a01, a0, spec);
      b0 = ((op_t *) srcp2)[0];
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      a00 = a01;
      mask = __builtin_e2k_pcmpeqb (b0, 0);
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

  __v2di a0, a00, a01, b0, spec;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  unsigned long int end_ptr = (srcp2 + len - 1) & ~15;
  unsigned int mask, mask0;

  /* длины больше порога */
  if ((srcp2 + len - 1) < srcp2)
    end_ptr = ~0UL << 4;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 16);
  tail = (srcp2 + len - 1) & 15;

  if (((srcp1 ^ srcp2) & 15) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~15;
    srcp2 &= ~15;
    a0 = ((__v2di *) srcp1)[0];
    b0 = ((__v2di *) srcp2)[0];
    srcp1 += 16;
    srcp2 += 16;
    __CMP0 (mask0, b0);
    __CMP (mask, a0, b0);
    mask |= ~((-1) << align2); /* вставляем 1 в каждый бит до начала строки */
    mask0 &= ((-1) << align2); /* обнуляем каждый бит до начала строки */

    if (mask == 0xffff && mask0 == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
      for (;;) {
        a0 = ((__v2di *) srcp1)[0];
        b0 = ((__v2di *) srcp2)[0];
        srcp1 += 16;
        srcp2 += 16;
        __CMP0 (mask0, b0);
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
    srcp1 += 16;
    srcp2 += 16;
    __CMP0 (mask0, b0);
    __CMP (mask, a0, b0);
    mask |= ~((-1) << align2); /* вставляем 1 в каждый бит до начала строки */
    mask0 &= ((-1) << align2); /* обнуляем каждый бит до начала строки */

    if (mask == 0xffff && mask0 == 0) {

      a00 = a01;

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
      for (;;) {
//        a01 = ((__v2di *) srcp1)[1];
        a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
        E2K_ALIGN_DATA128 (a00, a01, a0, spec);
        b0 = ((__v2di *) srcp2)[0];
        srcp1 += 16;
        srcp2 += 16;
        a00 = a01;
        __CMP0 (mask0, b0);
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
  align1 = __builtin_e2k_pandd (__builtin_e2k_pshufb (aa[1], aa[0], mask), 0xff);
  align2 = __builtin_e2k_pandd (__builtin_e2k_pshufb (bb[1], bb[0], mask), 0xff);
  return __builtin_e2k_psubw (align1, align2);


#else /* __iset__ > 5 */

#define __CMP(mask, x, y) /* bytes compare -> bit mask */             \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, y))
#define __CMP0(mask, x)   /* zero bytes -> bit mask */                \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __CMP_PRED(x, y)  /* bytes compare -> predicate */            \
    __builtin_e2k_qpcmpeqbap (x, y)
#define __CMP0_PRED(x)    /* zero bytes -> predicate */               \
    __builtin_e2k_qpcmpeqbop (x, qzero)

  __v2di a0, a00, a01, b0, spec, qpmask;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  unsigned long int end_ptr = (srcp2 + len - 1) & ~15;
  unsigned int mask, mask0;

  /* длины больше порога */
  if (end_ptr < srcp2)
    end_ptr = ~0UL << 4;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 16);
  tail = (srcp2 + len - 1) & 15;

  if (((srcp1 ^ srcp2) & 15) == 0) { /* обе строки выровнены одинаково */
    if (__builtin_expect ((E2K_BYTES_FROM_ALIGN (srcp1, 4096) > 4080) ||
                          (E2K_BYTES_FROM_ALIGN (srcp2, 4096) > 4080), 0)) { /* closely to page border */
      /* Offsets 4081-4095 will be shifted back to the aligned address thus fit into page */
      srcp1 &= ~15;
      srcp2 &= ~15;
      /* first qword loads are aligned */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
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
      srcp1 += 16 - align2;
      srcp2 += 16 - align2;
    }
    if (__CMP_PRED (a0, b0) && __CMP0_PRED (b0) == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
      for (;;) {
        a0 = ((__v2di *) srcp1)[0];
        b0 = ((__v2di *) srcp2)[0];
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
      srcp1 += 16 - align2;
      srcp2 += 16 - align2;
      srcp1 &= ~15;
      /* next qword loads are aligned */
      a00 = ((__v2di *) srcp1)[0];
    }
    if (__CMP_PRED (a0, b0) && __CMP0_PRED (b0) == 0) {

      /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
      for (;;) {
//        a01 = ((__v2di *) srcp1)[1];
        a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
        E2K_ALIGN_DATA128 (a00, a01, a0, spec);
        b0 = ((__v2di *) srcp2)[0];
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

libc_hidden_builtin_def (STRNCMP)
