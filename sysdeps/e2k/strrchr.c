/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strrchr.c 2068 2014-04-23 11:56:36Z vlog $
 */

#include <string.h>

#include <e2kintrin.h>

#undef strrchr

/* Find the last occurrence of C in S.  */
char *
strrchr (const char *str, int c_in)
{
  /* Set up a longword, each of whose bytes is C.  */
  unsigned long long charmask = __builtin_e2k_pshufb (c_in, c_in, 0);

#if __iset__ <= 4

#define FIND_CHAR(lword0, lword1) {                      \
  longword0 = __builtin_e2k_pcmpeqb (lword0, charmask);  \
  longword1 = __builtin_e2k_pcmpeqb (lword1, charmask);  \
  longword2 = __builtin_e2k_pcmpeqb (lword0, 0);         \
  longword3 = __builtin_e2k_pcmpeqb (lword1, 0);         \
  mask = __builtin_e2k_pmovmskb (longword1, longword0);  \
  mask0 = __builtin_e2k_pmovmskb (longword3, longword2); \
}

  const unsigned long long *longword_ptr, *longword_lastptr = NULL;
  unsigned long long longword0, longword1, longword2, longword3;
  unsigned int mask, mask0, align;

  align = E2K_BYTES_FROM_ALIGN (str, 16);
  longword_ptr = E2K_ALIGN_PTR_BACK (str, 16);

  FIND_CHAR (longword_ptr[0], longword_ptr[1]);
  mask >>= align;  /* обнуляем каждый бит до начала строки */
  mask0 >>= align;

  if (mask0 != 0) { /* нашли конец строки */
    /* Which of the bytes was the zero? */
    mask0 = __builtin_ctz (mask0);
    mask &= ~(-2 << mask0); /* обнуляем маску для символов после конца строки */
    return (mask == 0) ? NULL /* не нашли */ : (char *) str + 31 - __builtin_clz (mask);
  }
  longword_ptr += 2;

//FIXME: все эти извращения потому, что компилятор 24 ветки ставит лишнюю пересылку и цикл не влезает в 2 такта
// позже попробовать убрать
  if (mask != 0) longword_lastptr = longword_ptr;
  longword0 = longword1 = 0;

  /* We will test a 16 bytes at a time.  */
#pragma noprefetch
#pragma loop count (100) /* 2 такта, без apb */
  for (;;) {
    if ((longword1 | longword0) != 0) longword_lastptr = longword_ptr;
//    if (mask != 0) longword_lastptr = longword_ptr;
    FIND_CHAR (longword_ptr[0], longword_ptr[1]);
    longword_ptr += 2;
    if (mask0 != 0) break;
  }

  /* Which of the bytes was the zero? */
  mask0 = __builtin_ctz (mask0);
  mask &= ~(-2 << mask0); /* обнуляем маску для символов после конца строки */
  if (mask != 0) longword_lastptr = longword_ptr; /* найден символ в последнем longword-е */
  else if (longword_lastptr != NULL) { /* символ был найден ранее */
    longword0 = __builtin_e2k_pcmpeqb (longword_lastptr[-2], charmask);
    longword1 = __builtin_e2k_pcmpeqb (longword_lastptr[-1], charmask);
    mask = __builtin_e2k_pmovmskb (longword1, longword0);
  }
  else return NULL; /* не нашли */
  return (char *) longword_lastptr + 15 - __builtin_clz (mask);


#elif __iset__ <= 5

#define __CMP(mask, x)  /* bytes with 'c_in' -> bit mask */               \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))
#define __CMP0(mask, x) /* zero bytes -> bit mask */                      \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))

  const __v2di *qword_ptr, *qword_lastptr = NULL;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
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

  if (mask0 != 0) { /* нашли конец строки */
    /* Which of the bytes was the zero? */
    mask0 = __builtin_ctz (mask0);
    mask &= ~(-2 << mask0); /* обнуляем маску для символов после конца строки */
    return (mask == 0) ? NULL /* не нашли */ : (char *) str + 31 - __builtin_clz (mask);
  }
  qword_ptr += 2;

  /* We will test a 32 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 3 такта */
#pragma loop count (100)
  for (;;) {
      if (mask != 0) qword_lastptr = qword_ptr;
      qword0 = __builtin_e2k_qpcmpeqb (qword_ptr[0], qcharmask);
      qword1 = __builtin_e2k_qpcmpeqb (qword_ptr[1], qcharmask);
      qword2 = __builtin_e2k_qpcmpeqb (qword_ptr[0], qzero);
      qword3 = __builtin_e2k_qpcmpeqb (qword_ptr[1], qzero);
      mask = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpor (qword0, qword1));
      mask0 = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpor (qword2, qword3));
      qword_ptr += 2;
      if (mask0 != 0) break;
  }

  /* Which of the bytes was the zero? */
  mask = __builtin_e2k_qpsgn2mskb (qword0);
  mask1 = __builtin_e2k_qpsgn2mskb (qword1);
  mask0 = __builtin_e2k_qpsgn2mskb (qword2);
  mask2 = __builtin_e2k_qpsgn2mskb (qword3);
  mask |= mask1 << 16;
  mask0 |= mask2 << 16;
  mask0 = __builtin_ctz (mask0);
  mask &= ~(-2 << mask0); /* обнуляем маску для символов после конца строки */
  if (mask != 0) qword_lastptr = qword_ptr; /* найден символ в последнем qword-е */
  else if (qword_lastptr != NULL) { /* символ был найден ранее */
    __CMP (mask, qword_lastptr[-2]);
    __CMP (mask1, qword_lastptr[-1]);
    mask |= mask1 << 16;
  }
  else return NULL; /* не нашли */
  return (char *) qword_lastptr - 1 - __builtin_clz (mask);


#else /* __iset__ > 5 */

#define __CMP(mask, x)  /* bytes with 'c_in' -> bit mask */               \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qcharmask))
#define __CMP0(mask, x) /* zero bytes -> bit mask */                      \
    (mask) = __builtin_e2k_qpsgn2mskb (__builtin_e2k_qpcmpeqb (x, qzero))
#define __CMP_PRED(x)   /* bytes with 'c_in' -> predicate */              \
    __builtin_e2k_qpcmpeqbop (x, qcharmask)
#define __CMP0_PRED(x)  /* zero bytes -> predicate */                     \
    __builtin_e2k_qpcmpeqbop (x, qzero)

  const __v2di *qword_ptr, *qword_lastptr = NULL;
  const __v2di qzero = __builtin_e2k_qppackdl (0, 0);
  const __v2di qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
  __v2di qword;
  unsigned int mask, mask0, align;

  qword_ptr = E2K_ALIGN_PTR_BACK (str, 16);
  qword_ptr++;

  if (__builtin_expect (E2K_BYTES_FROM_ALIGN (str, 4096) > 4080, 0)) { /* closely to page border */
    /* Offsets 4081-4095 will be shifted back to the aligned address thus fit into page */
    align = E2K_BYTES_FROM_ALIGN (str, 16);
    /* first qword load is aligned */
    qword = qword_ptr[-1];
    __CMP (mask, qword);
    __CMP0 (mask0, qword);
    mask >>= align;  /* обнуляем каждый бит до начала строки */
    mask0 >>= align;

    if (mask0 != 0) { /* нашли конец строки */
      /* Which of the bytes was the zero? */
      mask0 = __builtin_ctz (mask0);
      mask &= ~(-2 << mask0); /* обнуляем маску для символов после конца строки */
      return (mask == 0) ? NULL /* не нашли */ : (char *) str + 31 - __builtin_clz (mask);
    }
    if (mask != 0) qword_lastptr = qword_ptr;
  }
  else {
    /* first qword load is unaligned */
    qword = ((__v2di *) str)[0];

    if (__CMP0_PRED (qword)) { /* нашли конец строки */
      __CMP (mask, qword);
      __CMP0 (mask0, qword);
      /* Which of the bytes was the zero? */
      mask0 = __builtin_ctz (mask0);
      mask &= ~(-2 << mask0); /* обнуляем маску для символов после конца строки */
      return (mask == 0) ? NULL /* не нашли */ : (char *) str + 31 - __builtin_clz (mask);
    }
    if (__CMP_PRED (qword)) qword_lastptr = qword_ptr;
  }

  qword = *qword_ptr++;
  if (!(__CMP0_PRED (qword))) {

  /* We will test a 16 bytes at a time.  */
#pragma noprefetch /* иначе применяется apb, 1 такт */
#pragma loop count (100)
    for (;;) {
      if (__CMP_PRED (qword)) qword_lastptr = qword_ptr;
      qword = *qword_ptr++;
      if (__CMP0_PRED (qword)) break;
    }
  }
  /* Which of the bytes was the zero? */
  __CMP (mask, qword);
  __CMP0 (mask0, qword);
  mask0 = __builtin_ctz (mask0);
  mask &= ~(-2 << mask0); /* обнуляем маску для символов после конца строки */
  if (mask != 0) qword_lastptr = qword_ptr; /* найден символ в последнем qword-е */
  else if (qword_lastptr != NULL) { /* символ был найден ранее */
    __CMP (mask, qword_lastptr[-1]);
  }
  else return NULL; /* не нашли */
  return (char *) qword_lastptr + 15 - __builtin_clz (mask);

#endif /* __iset__ > 5 */

}

#ifdef weak_alias
#undef rindex
weak_alias (strrchr, rindex)
#endif
libc_hidden_builtin_def (strrchr)
