/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strcmp.c 2218 2014-06-16 13:57:59Z vlog $
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

#undef strcmp

/* Compare S1 and S2, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2.  */
int
strcmp (p1, p2)
     const char *p1;
     const char *p2;
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned long int srcp1 = (unsigned long int) s1;
  unsigned long int srcp2 = (unsigned long int) s2;
  op_t align1, align2;

#if __iset__ <= 4

  if (s1[0] == '\0' || s1[0] != s2[0]) return s1[0] - s2[0];
  if (s1[1] == '\0' || s1[1] != s2[1]) return s1[1] - s2[1];
  srcp1 += 2;
  srcp2 += 2;

  op_t a0, a00, a01, b0, spec, mask;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 8);
  mask = ((-1LL) << (align2 * 8));

  if (((srcp1 ^ srcp2) & 7) == 0) { /* обе строки выровнены одинаково */
    srcp1 &= ~7;
    srcp2 &= ~7;
    a0 = ((op_t *) srcp1)[0] | ~mask; /* байты до начала строки заполняем 0xff */
    b0 = ((op_t *) srcp2)[0] | ~mask;
    srcp1 += OPSIZ;
    srcp2 += OPSIZ;
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    if (mask != 0 || a0 != b0) goto m_last;

    /* We will test a 8 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
    for (;;) {
      a0 = ((op_t *) srcp1)[0];
      b0 = ((op_t *) srcp2)[0];
      srcp1 += OPSIZ;
      srcp2 += OPSIZ;
      mask = __builtin_e2k_pcmpeqb (b0, 0);
      if (mask != 0 || a0 != b0) break;
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
    mask = __builtin_e2k_pcmpeqb (b0, 0);
    if (mask != 0 || a0 != b0) goto m_last;

    a00 = a01;

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
      if (mask != 0 || a0 != b0) break;
    }
  }
m_last:
  spec = __builtin_ctzll (mask | ~__builtin_e2k_pcmpeqb (a0, b0));
  align1 = (a0 >> spec) & 0xff;
  align2 = (b0 >> spec) & 0xff;
  return align1 - align2;


#elif __iset__ <= 5

#if 0
/* этот вариант делает цикл на 1 операцию меньше, но задержка увеличивается */
#define __CMP(mask, x, y) { /* find different chars or zero byte */ \
  __v2di c0 = __builtin_e2k_qpcmpeqb (x, y);                        \
  __v2di c1 = __builtin_e2k_qpcmpeqb (y, qzero);                    \
  mask = __builtin_e2k_qpsgn2mskb ( __builtin_e2k_qpsubb (c0, c1)); \
}
  if (mask != 0xffff) goto m_last;
m_last:
  mask = __builtin_ctz (~mask);
#endif

#define __CMP(mask, x, y) /* bytes compare -> bit mask */             \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, y))
#define __CMP0(mask, x)   /* zero bytes -> bit mask */                \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))

  if (s1[0] == '\0' || s1[0] != s2[0]) return s1[0] - s2[0];
  if (s1[1] == '\0' || s1[1] != s2[1]) return s1[1] - s2[1];
  srcp1 += 2;
  srcp2 += 2;

  __v2di a0, a00, a01, b0, spec;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  unsigned int mask, mask0;

  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 16);

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
    if (mask != 0xffff || mask0 != 0) goto m_last;

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
      if (mask != 0xffff || mask0 != 0) break;
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
    srcp1 += 16;
    srcp2 += 16;
    __CMP0 (mask0, b0);
    __CMP (mask, a0, b0);
    mask |= ~((-1) << align2); /* вставляем 1 в каждый бит до начала строки */
    mask0 &= ((-1) << align2); /* обнуляем каждый бит до начала строки */
    if (mask != 0xffff || mask0 != 0) goto m_last;

    a00 = a01;

    /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
    for (;;) {
//      a01 = ((__v2di *) srcp1)[1];
      a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
      E2K_ALIGN_DATA128 (a00, a01, a0, spec);
      b0 = ((__v2di *) srcp2)[0];
      srcp1 += 16;
      srcp2 += 16;
      a00 = a01;
      __CMP0 (mask0, b0);
      __CMP (mask, a0, b0);
      if (mask != 0xffff || mask0 != 0) break;
    }
  }
m_last:
  mask = __builtin_ctz (mask0 | ~mask);
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
  unsigned int mask, mask0;

  align1 = E2K_BYTES_FROM_ALIGN (srcp1, 16);
  align2 = E2K_BYTES_FROM_ALIGN (srcp2, 16);

  if (align1 == align2) { /* обе строки выровнены одинаково */
    if (__builtin_expect ((E2K_BYTES_FROM_ALIGN (srcp1, 4096) > 4080) ||
                          (E2K_BYTES_FROM_ALIGN (srcp2, 4096) > 4080), 0)) { /* closely to page border */
      /* Offsets 4081-4095 will be shifted back to the aligned address thus fit into page */
      srcp1 &= ~15;
      srcp2 &= ~15;
      /* first qword loads are aligned */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      qpmask = __builtin_e2k_qpmsk2sgnb (qzero, (1 << align2) - 1);
      qpmask = __builtin_e2k_qpcmpgtb (qzero, qpmask);
      a0 = __builtin_e2k_qpor (a0, qpmask); /* вставляем 0xff в каждый байт до начала строки */
      b0 = __builtin_e2k_qpor (b0, qpmask);
      if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0)) goto m_last;
      srcp1 += 16;
      srcp2 += 16;
    }
    else {
      /* first qword loads are unaligned */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0)) goto m_last;
      srcp1 += 16 - align2;
      srcp2 += 16 - align2;
    }

    /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
    for (;;) {
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      srcp1 += 16;
      srcp2 += 16;
      if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0)) break;
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
      if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0)) goto m_last;
      a00 = a01;
      srcp1 += 16;
      srcp2 += 16;
    }
    else {
      /* first qword loads are unaligned */
      a0 = ((__v2di *) srcp1)[0];
      b0 = ((__v2di *) srcp2)[0];
      if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0)) goto m_last;
      srcp1 += 16 - align2;
      srcp2 += 16 - align2;
      srcp1 &= ~15;
      /* next qword loads are aligned */
      a00 = ((__v2di *) srcp1)[0];
    }

    /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 2 такта */
#pragma loop count (100)
    for (;;) {
//      a01 = ((__v2di *) srcp1)[1];
      a01 = __builtin_e2k_ld_128_cleartag ((__v2di *) srcp1, 16);
      E2K_ALIGN_DATA128 (a00, a01, a0, spec);
      b0 = ((__v2di *) srcp2)[0];
      srcp1 += 16;
      srcp2 += 16;
      a00 = a01;
      if (__CMP_PRED (a0, b0) == 0 || __CMP0_PRED (b0)) break;
    }
  }
m_last:
  __CMP0 (mask0, b0);
  __CMP (mask, a0, b0);
  mask = __builtin_ctz (mask0 | ~mask);
  __v2di aa = a0;
  __v2di bb = b0;
  align1 = __builtin_e2k_pandd (__builtin_e2k_pshufb (aa[1], aa[0], mask), 0xff);
  align2 = __builtin_e2k_pandd (__builtin_e2k_pshufb (bb[1], bb[0], mask), 0xff);
  return __builtin_e2k_psubw (align1, align2);

#endif /* __iset__ > 5 */

}

libc_hidden_builtin_def (strcmp)
