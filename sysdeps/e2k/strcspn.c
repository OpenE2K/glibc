/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: strcspn.c 2101 2014-05-13 11:24:32Z vlog $
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
#include <e2kintrin.h>

#undef strcspn

/* Set up a longword, each of whose bytes is C.  */
#define EXTEND_CHAR(dst, c) \
  (dst) = __builtin_e2k_pshufb (c, c, 0)

/* Return the length of the maximum initial segment of S
   which contains no characters from REJECT.  */
size_t
strcspn (s, reject)
     const char *s;
     const char *reject;
{
  unsigned long long table[4], table0 = 1, table1 = 0, table2 = 0, table3 = 0;
  unsigned long long spec, val, charmask;
  int i, len2;
  const char *p = s;
  unsigned char c;

  len2 = strlen (reject);

#if __iset__ <= 5

  if (len2 <= 8) { /* длина reject <= 8 */

    unsigned long long longword2, mask;
    const unsigned long long *longword_ptr2 = E2K_ALIGN_PTR_BACK (reject, 8);
    size_t align2 = E2K_BYTES_FROM_ALIGN (reject, 8);

    longword2 = *longword_ptr2;

    if (align2) {
      unsigned long long longword21 = __builtin_e2k_ld_64s_cleartag (longword_ptr2, 8);
      E2K_PREPARE_ALIGN (align2, spec);
      E2K_ALIGN_DATA (longword2, longword21, longword2, spec);
    }
    if (len2 < 8) {
      /* вставляем 0 в каждый байт после конца строки */
      longword2 &= ~((-1LL) << (len2 * 8));
    }

#pragma noprefetch
#pragma loop count (15) /* 1 такт, без apb */
    for (;;) {
      c = *p++;
      EXTEND_CHAR (charmask, c);
      mask = __builtin_e2k_pcmpeqb (longword2, charmask);
      if (c == '\0' || mask != 0) break;
    }

#else /* __iset__ > 5 */

  if (len2 <= 16) { /* длина reject <= 16 */

    __v2di qword2, qspec, qcharmask;
    const __v2di qzero = __builtin_e2k_qppackdl (0, 0);

    if (__builtin_expect ((E2K_BYTES_FROM_ALIGN (reject, 4096) > 4080), 0)) { /* closely to page border */
      /* qword load is unaligned, till the string end */
      qword2 = ((__v2di *) ((const char *) reject - (16 - len2)))[0];

      /* вставляем 0 в каждый байт после конца строки */
      E2K_PREPARE_ALIGN128 (16 - len2, qspec);
      E2K_ALIGN_DATA128 (qword2, qzero, qword2, qspec);
    }
    else {
      /* qword load is unaligned, from the string beginning */
      qword2 = *((__v2di *) reject);

      /* вставляем 0 в каждый байт до начала строки */
      E2K_PREPARE_ALIGN128 (len2, qspec);
      E2K_ALIGN_DATA128 (qzero, qword2, qword2, qspec);
    }

#pragma noprefetch
#pragma loop count (15) /* 1 такт, без apb */
    for (;;) {
      c = *p++;
      EXTEND_CHAR (charmask, c);
      qcharmask = __builtin_e2k_qppackdl (charmask, charmask);
      if (c == '\0' || __builtin_e2k_qpcmpeqbop (qword2, qcharmask) != 0) break;
    }

#endif /* __iset__ > 5 */

    return p - s - 1;
  }

  /* длина reject > 8(16) */
#pragma noprefetch
#pragma unroll (1) /* 3 такта, без apb */
#pragma loop count (15)
  for (i = 0; i < len2; i++) { /* заполняем таблицу символами из reject */
    spec = ((unsigned char *) reject)[i];
    val = 1LL << spec;
    if (spec & 0x80) {
      if (spec & 0x40) table3 |= val;
      else table2 |= val;
    }
    else {
      if (spec & 0x40) table1 |= val;
      else table0 |= val;
    }
  }
  table[0] = table0;
  table[1] = table1;
  table[2] = table2;
  table[3] = table3;

#pragma noprefetch
#pragma loop count (10) /* 1 такт, без apb */
  for (;;) { /* обрабатываем по 1 байту из s */
    spec = *((unsigned char *) p);
    p++;
    if (*((unsigned long long *) ((char *) table + ((spec >> 3) & 0x18))) & (1LL << spec))
      break;
  }
  return p - 1 - s;
}

libc_hidden_builtin_def (strcspn)
