/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

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

#undef memset

/* ************************************************************************** */

#define WC_LIMIT SIZE_L2

/* ************************************************************************** */

void *memset(void *mem, int c, size_t len)
{
    unsigned char * ac = (unsigned char *) mem;
    long i, ll;

#if __iset__ <= 4

    long long * al, cc;

    if (len > 0) ac[0] = c;
    if (len > 1) ac[1] = c;
    if (len > 2) ac[2] = c;
    if (len > 3) ac[3] = c;
    if (len > 4) ac[4] = c;
    if (len > 5) ac[5] = c;
    if (len > 6) ac[6] = c;
    if (len > 7) ac[7] = c;
    if (len <= 8)
        return mem;

    cc = __builtin_e2k_pshufb (c, c, 0); /* 00000000 */

    /* выше заполнили невыровненную часть */
    len -= 8 - ((size_t) ac & 7);
    al = (long long *) (((long) ac + 8) & ~7);

    {
        ll = (len >> 3) & ~1;
        ac = (unsigned char *) al + (len & ~15);

        /* чтобы компилятор не умничал сразу заполняем хвост.
           это может быть не очень хорошо для кеша. */
        if (len & 8)
            *(long long *) ac = cc;

        if (len & 4)
            *(int *) (ac + (len & 8)) = cc;

        if (len & 2)
            *(short *) (ac + (len & 12)) = cc;

        if (len & 1)
            *(ac + (len & 14)) = c;

        /* заполняем тело по 2*8 байт */
        if (ll < WC_LIMIT / 8) {       /* массив влезает в L2/L3 кэш */

            for (i = 0; i < ll; i += 2)
            {
                al[i] = cc;
                al[i + 1] = cc;
            }
            return mem;
        }
        else { /* массив никуда не влезает */

#ifdef __ptr64__

#define BIGSIZE 0x10000000

            int ii, isize;

#pragma loop count (1)
            for (i = 0; i < ll; i += BIGSIZE) {
                isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma loop count (1000)
                for (ii = 0; ii < isize; ii += 2)
                {
                    __builtin_e2k_st_64s_nt (cc, (long long *) al + ii);
                    __builtin_e2k_st_64s_nt (cc, (long long *) al + ii + 1);
                }
                al += isize;
            }

#else  /* __ptr64__ */

#pragma loop count (1000)
            for (i = 0; i < ll; i += 2)
            {
                __builtin_e2k_st_64s_nt (cc, (long long *) al + i);
                __builtin_e2k_st_64s_nt (cc, (long long *) al + i + 1);
            }

#endif /* __ptr64__ */

            return mem;
        }
    }


#else /* __iset__ > 4 */

    long long cc = __builtin_e2k_pshufb (c, c, 0); /* 00000000 */
    __v2di qval = __builtin_e2k_qppackdl (cc, cc);

#if __iset__ <= 5

    __v2di *qp = (__v2di *) ((size_t) ac & ~15);
    size_t len1, diff;

    diff = (size_t) ac & 15;
    int mask = 0xffff << diff;
    len1 = len - (16 - diff);

    if (len <= (16 - diff))
        mask &= 0xffff >> (-len1);
    __builtin_e2k_pst_128 (qval, qp, mask);
    if (len <= (16 - diff))
        return mem;

    qp++;
    /* заполняем хвост */
    ll = len1 >> 4;
    mask = (1 << (len1 & 15)) - 1;
    __builtin_e2k_pst_128 (qval, (__v2di *) ((char *) qp + (len1 & ~15)), mask);

#else /* __iset__ > 5 */

    __v2di *qp = (__v2di *) ac;

    if (len <= 16) {
        __builtin_e2k_pst_128 (qval, qp, 0xffff >> (16 - len));
        return mem;
    }

    /* заполняем хвост */
    ll = len >> 4;
    int mask = (1 << (len & 15)) - 1;
    __builtin_e2k_pst_128 (qval, (__v2di *) ((char *) qp + (len & ~15)), mask);

#endif /* __iset__ > 5 */

    /* заполняем тело по 2*8 байт */
    if (ll < WC_LIMIT / 8 / 2) { /* массив влезает в L2 кэш */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
        for (i = 0; i < ll; i++) {
            qp[i] = qval;
        }
        return mem;

    } else { /* массив никуда не влезает */

#ifdef __ptr64__

#define BIGSIZE 0x8000000

        int ii, isize;

#pragma loop count (1)
        for (i = 0; i < ll; i += BIGSIZE) {
            isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
            for (ii = 0; ii < isize; ii++)
            {
                __builtin_e2k_st_128_nt (qval, qp + ii);
            }
            qp += isize;
        }

#else  /* __ptr64__ */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
        for (i = 0; i < ll; i++) {
            __builtin_e2k_st_128_nt (qval, qp + i);
        }

#endif /* __ptr64__ */

        return mem;
    }

#endif /* __iset__ > 4 */

}

libc_hidden_builtin_def (memset)
