/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Torbjorn Granlund (tege@sics.se).

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

#undef __bzero

/* Сделано из memset.c */

/* ************************************************************************** */

#define WC_LIMIT SIZE_L2

/* ************************************************************************** */

/* Set N bytes of S to 0.  */
void
__bzero (s, len)
     void *s;
     size_t len;
{
    unsigned char * ac = (unsigned char *) s;
    long i, ll;

#if __iset__ <= 4

    long long * al;

    if (len > 0) ac[0] = 0;
    if (len > 1) ac[1] = 0;
    if (len > 2) ac[2] = 0;
    if (len > 3) ac[3] = 0;
    if (len > 4) ac[4] = 0;
    if (len > 5) ac[5] = 0;
    if (len > 6) ac[6] = 0;
    if (len > 7) ac[7] = 0;
    if (len <= 8)
        return;

    /* выше заполнили невыровненную часть */
    len -= 8 - ((size_t) ac & 7);
    al = (long long *) (((long) ac + 8) & ~7);

    {
        ll = (len >> 3) & ~1;
        ac = (unsigned char *) al + (len & ~15);

        /* чтобы компилятор не умничал сразу заполняем хвост.
           это может быть не очень хорошо для кеша. */
        if (len & 8)
            *(long long *) ac = 0;

        if (len & 4)
            *(int *) (ac + (len & 8)) = 0;

        if (len & 2)
            *(short *) (ac + (len & 12)) = 0;

        if (len & 1)
            *(ac + (len & 14)) = 0;

        /* заполняем тело по 2*8 байт */
        if (ll < WC_LIMIT / 8) {       /* массив влезает в L2/L3 кэш */

            for (i = 0; i < ll; i += 2)
            {
                al[i] = 0;
                al[i + 1] = 0;
            }
            return;
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
                    __builtin_e2k_st_64s_nt (0, (long long *) al + ii);
                    __builtin_e2k_st_64s_nt (0, (long long *) al + ii + 1);
                }
                al += isize;
            }

#else  /* __ptr64__ */

#pragma loop count (1000)
            for (i = 0; i < ll; i += 2)
            {
                __builtin_e2k_st_64s_nt (0, (long long *) al + i);
                __builtin_e2k_st_64s_nt (0, (long long *) al + i + 1);
            }

#endif /* __ptr64__ */

            return;
        }
    }


#else /* __iset__ > 4 */

    __v2di qval = (__v2di) {0, 0};

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
        return;

    qp++;
    /* заполняем хвост */
    ll = len1 >> 4;
    mask = (1 << (len1 & 15)) - 1;
    __builtin_e2k_pst_128 (qval, (__v2di *) ((char *) qp + (len1 & ~15)), mask);

#else /* __iset__ > 5 */

    __v2di *qp = (__v2di *) ac;

    if (len <= 16) {
        __builtin_e2k_pst_128 (qval, qp, 0xffff >> (16 - len));
        return;
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

    }

#endif /* __iset__ > 4 */

}

weak_alias (__bzero, bzero)
