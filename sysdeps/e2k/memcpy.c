/* Copyright (c) 2015-2018 ZAO "MCST". All rights reserved.
 *
 * @(#) $Id: memchr.c 2068 2014-04-23 11:56:36Z vlog $
 */

/* Copy memory to memory until the specified number of bytes
   has been copied.  Overlap is NOT handled correctly.
   Copyright (C) 1991, 1997, 2003 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <string.h>
#include <e2kintrin.h>

#if     !defined(RETURN)
#undef memcpy
#define RETURN(p,shift) return (void*)(p) /* Return dst */
#endif

#define restrict __restrict__

/* ************************************************************************** */

#define WC_LIMIT SIZE_L2

/* ************************************************************************** */

void *memcpy(void * restrict dst, const void * restrict src, size_t len)
{
    unsigned char * restrict ac = (unsigned char *) dst;
    unsigned char * restrict bc = (unsigned char *) src;
    long i, ll;

#if __iset__ <= 4

    if (len > 0) ac[0] = bc[0];
    if (len > 1) ac[1] = bc[1];
    if (len > 2) ac[2] = bc[2];
    if (len > 3) ac[3] = bc[3];
    if (len > 4) ac[4] = bc[4];
    if (len > 5) ac[5] = bc[5];
    if (len > 6) ac[6] = bc[6];
    if (len > 7) ac[7] = bc[7];
    if (len <= 8)
        RETURN (dst, len);

    /* выше скопировали невыровненную часть */
    size_t len1 = len - (8 - ((size_t) ac & 7));
    bc += 8 - ((size_t) ac & 7);
    ac += 8 - ((size_t) ac & 7);

    {
        unsigned long long * restrict al = (unsigned long long *) ac;
        unsigned long long * restrict bl = (unsigned long long *) ((long) bc & ~7);
        unsigned long long shift = (unsigned long long) (unsigned long) bc & 7;
        unsigned long long res;

        /* копируем хвост */
        ac += len1 & ~7;
        bc += len1 & ~7;

        if (len1 & 4) {
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
            ((short *) ac)[1] = (bc[3] << 8) | bc[2];
            bc += 4;
            ac += 4;
        }
        if (len1 & 2) {
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
            bc += 2;
            ac += 2;
        }
        if (len1 & 1) {
            ac[0] = bc[0];
        }

        /* копируем основную часть двойными словами */
        E2K_PREPARE_ALIGN (shift, shift);
        ll = len1 >> 3;

        if (ll > 0) { /* копируем последний дабл с залезанием за границы массива */
            res = __builtin_e2k_ld_64s_cleartag (bl + ll, 0);
            E2K_ALIGN_DATA (bl[ll - 1], res, al[ll - 1], shift);
        }
        if (ll > 1) E2K_ALIGN_DATA (bl[0], bl[1], al[0], shift);
        if (ll > 2) E2K_ALIGN_DATA (bl[1], bl[2], al[1], shift);
        if (ll > 3) E2K_ALIGN_DATA (bl[2], bl[3], al[2], shift);
        if (ll <= 4)
            RETURN (dst, len);

        if (ll < 64) {       /* обрабатываем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
            for (i = 3; i < ll - 1; i++)
            {
                E2K_ALIGN_DATA (bl[i], bl[i + 1], al[i], shift);
            }
        }
        else if (ll < WC_LIMIT / 8 / 2) {       /* массив влезает в половину L2/L3 кэша */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
            for (i = 3; i < ll - 1; i++)
            {
                E2K_ALIGN_DATA (bl[i], bl[i + 1], al[i], shift);
            }
        }
        else { /* массив никуда не влезает */

#ifdef __ptr64__

#define BIGSIZE 0x10000000

            int ii, isize;

            al += 3;
            bl += 3;
            ll -= 4;

#pragma loop count (1)
            for (i = 0; i < ll; i += BIGSIZE) {
                isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                for (ii = 0; ii < isize; ii++)
                {
                    E2K_ALIGN_DATA (bl[ii], bl[ii + 1], res, shift);
                    __builtin_e2k_st_64s_nt (res, al + ii);
                }
                al += isize;
                bl += isize;
            }

#else  /* __ptr64__ */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
            for (i = 3; i < ll - 1; i++)
            {
                E2K_ALIGN_DATA (bl[i], bl[i + 1], res, shift);
                __builtin_e2k_st_64s_nt (res, al + i);
            }

#endif /* __ptr64__ */

        }
    }


#elif __iset__ <= 5

    __v2di * restrict qsp, qsrc0, qsrc1, qsrc, qshift;
    __v2di * restrict qdp = (__v2di *) ((size_t) ac & ~15);
    long shift, diff;

    diff = (size_t) ac & 15;
    int mask = 0xffff << diff;
    size_t len1 = len - (16 - diff);
    shift = ((size_t) bc - diff) & 15;
    qsp = (__v2di *) (((size_t) bc - diff) & ~15);
//    E2K_PREPARE_ALIGN128 (shift, qshift);
    qshift = __builtin_e2k_qppackdl (__builtin_e2k_pshufb (0, shift, 0), __builtin_e2k_pshufb (0, shift, 0));
    qshift = __builtin_e2k_qpaddd (qshift, (__v2di) {0x0706050403020100LL, 0x0f0e0d0c0b0a0908LL});

    qsrc0 = __builtin_e2k_ld_128_cleartag (qsp, 0);
    qsrc1 = __builtin_e2k_ld_128_cleartag (qsp, 16);
    E2K_ALIGN_DATA128 (qsrc0, qsrc1, qsrc, qshift);
    if (len <= (16 - diff))
        mask &= 0xffff >> (-len1);
    __builtin_e2k_pst_128 (qsrc, qdp, mask);
    if (len <= (16 - diff))
        RETURN (dst, len);

    diff = (-len1) & 15; /* вычисляем и копируем хвост из diff байт (от 1 до 16) */
    mask = 0xffff >> diff;
    ll = (len1 - (16 - diff)) >> 4;
    qsp++;
    qdp++;
    qsrc0 = qsp[ll];
    qsrc1 = __builtin_e2k_ld_128_cleartag (qsp + ll + 1, 0);
    E2K_ALIGN_DATA128 (qsrc0, qsrc1, qsrc, qshift);
    __builtin_e2k_pst_128 (qsrc, qdp + ll, mask);

    /* открутка первых 4-х итераций (64 байта) */
    if (ll > 0) E2K_ALIGN_DATA128 (qsp[0], qsp[1], qdp[0], qshift);
    if (ll > 1) E2K_ALIGN_DATA128 (qsp[1], qsp[2], qdp[1], qshift);
    if (ll > 2) E2K_ALIGN_DATA128 (qsp[2], qsp[3], qdp[2], qshift);
    if (ll > 3) E2K_ALIGN_DATA128 (qsp[3], qsp[4], qdp[3], qshift);
    qsp += 4;
    qdp += 4;
    ll -= 4;

    /* ручной сплит векторов */
    __v2di * restrict qsp1 = qsp + (ll >> 1);
    __v2di * restrict qdp1 = qdp + (ll >> 1);

    /* копируем основную часть квадро регистрами */
    if (ll <= 16) { /* до 256 байт копируем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
#pragma noprefetch /* иначе применяется apb */
        for (i = 0; i < ll; i++) {
            E2K_ALIGN_DATA128 (qsp[i], qsp[i + 1], qdp[i], qshift);
        }

    } else if (ll < WC_LIMIT / 16 / 2) { /* массив влезает в половину L2 кэша */

#pragma unroll (1) /* 1 такт */
#pragma loop count (1000)
        for (i = 0; i < (ll >> 1); i++) {
            E2K_ALIGN_DATA128 (qsp[i], qsp[i + 1], qdp[i], qshift);
            E2K_ALIGN_DATA128 (qsp1[i], qsp1[i + 1], qdp1[i], qshift);
        }
        if (ll & 1) {
            E2K_ALIGN_DATA128 (qsp[ll - 1], qsp[ll], qdp[ll - 1], qshift);
        }

    } else { /* массив никуда не влезает */

#ifdef __ptr64__

#define BIGSIZE 0x8000000

        int ii, isize;

#pragma loop count (1)
        for (i = 0; i < (ll >> 1); i += isize) {
            isize = ((ll >> 1) - i) > BIGSIZE ? BIGSIZE : (ll >> 1) - i;

#pragma unroll (1) /* 1 такт */
#pragma loop count (1000)
            for (ii = 0; ii < isize; ii++)
            {
                E2K_ALIGN_DATA128 (qsp[ii], qsp[ii + 1], qsrc, qshift);
                __builtin_e2k_st_128_nt (qsrc, qdp + ii);
                E2K_ALIGN_DATA128 (qsp1[ii], qsp1[ii + 1], qsrc, qshift);
                __builtin_e2k_st_128_nt (qsrc, qdp1 + ii);
            }
            qsp += isize;
            qdp += isize;
            qsp1 += isize;
            qdp1 += isize;
        }
        if (ll & 1) {
            E2K_ALIGN_DATA128 (qsp1[0], qsp1[1], qsrc, qshift);
            __builtin_e2k_st_128_nt (qsrc, qdp1);
        }

#else  /* __ptr64__ */

#pragma unroll (1) /* 1 такт */
#pragma loop count (1000)
        for (i = 0; i < (ll >> 1); i++) {
            E2K_ALIGN_DATA128 (qsp[i], qsp[i + 1], qsrc, qshift);
            __builtin_e2k_st_128_nt (qsrc, qdp + i);
            E2K_ALIGN_DATA128 (qsp1[i], qsp1[i + 1], qsrc, qshift);
            __builtin_e2k_st_128_nt (qsrc, qdp1 + i);
        }
        if (ll & 1) {
            E2K_ALIGN_DATA128 (qsp[ll - 1], qsp[ll], qsrc, qshift);
            __builtin_e2k_st_128_nt (qsrc, qdp + ll - 1);
        }

#endif /* __ptr64__ */

    }


#else /* __iset__ > 5 */

    __v2di * restrict qdp = (__v2di *) ac;
    __v2di * restrict qsp = (__v2di *) bc;
    __v2di qsrc;
    long diff;

    if (len <= 16) {
        if (__builtin_expect (E2K_BYTES_FROM_ALIGN (qsp, 4096) > 4080, 0)) { /* closely to page border */
            /* Offsets 4081-4095 will be shifted back from the array last byte thus fit into page */
            qsrc = ((__v2di *) ((const char *) qsp - (16 - len)))[0];
            __builtin_pstoremas_128v (qsrc, (char *) qdp - (16 - len), 0xffff << (16 - len), __LCC_MAS_SPEC, __LCC_CHAN_ANY);
        }
        else {
            __builtin_pstoremas_128v (qsp[0], qdp, 0xffff >> (16 - len), __LCC_MAS_SPEC, __LCC_CHAN_ANY);
        }
        RETURN (dst, len);
    }

    diff = (-len) & 15; /* вычисляем и копируем хвост из diff байт (от 1 до 16) */
    ll = (len - (16 - diff)) >> 4;
    qsrc = ((__v2di *) ((const char *) qsp - (16 - len)))[0];
    __builtin_e2k_pst_128 (qsrc, (char *) qdp - (16 - len), 0xffff << diff);

    /* открутка первых 4-х итераций (64 байта) */
    if (ll > 0) qdp[0] = qsp[0];
    if (ll > 1) qdp[1] = qsp[1];
    if (ll > 2) qdp[2] = qsp[2];
    if (ll > 3) qdp[3] = qsp[3];
    if (ll <= 4)
        RETURN (dst, len);
    qsp += 4;
    qdp += 4;
    ll -= 4;

    /* ручной сплит векторов */
    __v2di * restrict qsp1 = qsp + (ll >> 1);
    __v2di * restrict qdp1 = qdp + (ll >> 1);

    /* копируем основную часть квадро регистрами */
    if (ll <= 16) { /* до 256 байт копируем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
#pragma noprefetch /* иначе применяется apb */
        for (i = 0; i < ll; i++) {
            qdp[i] = qsp[i];
        }

    } else if (ll < WC_LIMIT / 16 / 2) { /* массив влезает в половину L2 кэша */

#pragma unroll (1) /* 1 такт */
#pragma loop count (1000)
        for (i = 0; i < (ll >> 1); i++) {
            qdp[i] = qsp[i];
            qdp1[i] = qsp1[i];
        }
        if (ll & 1) {
            qdp[ll - 1] = qsp[ll - 1];
        }

    } else { /* массив никуда не влезает */

#ifdef __ptr64__

#define BIGSIZE 0x8000000

        int ii, isize;

#pragma loop count (1)
        for (i = 0; i < (ll >> 1); i += isize) {
            isize = ((ll >> 1) - i) > BIGSIZE ? BIGSIZE : (ll >> 1) - i;

#pragma unroll (1) /* 1 такт */
#pragma loop count (1000)
            for (ii = 0; ii < isize; ii++)
            {
                __builtin_e2k_st_128_nt (qsp[ii], qdp + ii);
                __builtin_e2k_st_128_nt (qsp1[ii], qdp1 + ii);
            }
            qsp += isize;
            qdp += isize;
            qsp1 += isize;
            qdp1 += isize;
        }
        if (ll & 1) {
            __builtin_e2k_st_128_nt (qsp1[0], qdp1);
        }

#else  /* __ptr64__ */

#pragma unroll (1) /* 1 такт */
#pragma loop count (1000)
        for (i = 0; i < (ll >> 1); i++) {
            __builtin_e2k_st_128_nt (qsp[i], qdp + i);
            __builtin_e2k_st_128_nt (qsp1[i], qdp1 + i);
        }
        if (ll & 1) {
            __builtin_e2k_st_128_nt (qsp[ll - 1], qdp + ll - 1);
        }

#endif /* __ptr64__ */

    }

#endif /* __iset__ > 5 */

    RETURN (dst, len);
}

libc_hidden_builtin_def (memcpy)
