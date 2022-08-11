/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/* Copy memory to memory until the specified number of bytes
   has been copied.  Overlap is handled correctly.
   Copyright (C) 1991, 1995, 1996, 1997, 2003 Free Software Foundation, Inc.
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

/* All this is so that bcopy.c can #include
   this file after defining some things.  */
#ifndef a1
#define a1      dst     /* First arg is DST.  */
#define a1const
#define a2      src     /* Second arg is SRC.  */
#define a2const const
#undef memmove
#endif
#if     !defined(RETURN) || !defined(rettype)
#define RETURN(s)       return (s)      /* Return DST.  */
#define rettype         void *
#endif

#define restrict __restrict__

/* ************************************************************************** */

#if defined (__elbrus_8c__) || __iset__ > 4
#define WC_LIMIT SIZE_L3 /* в П1 появился L3 кэш */
#else /* defined (__elbrus_8c__) || __iset__ > 4 */
#define WC_LIMIT SIZE_L2
#endif /* defined (__elbrus_8c__) || __iset__ > 4 */

/* ************************************************************************** */

#if __iset__ <= 4

#define MAKE(i) __builtin_e2k_pshufb (bl[-(i)], bl[-(i) - 1], shift)
#define PREPARE shift = __builtin_e2k_psubd (0x0f0e0d0c0b0a0908LL, __builtin_e2k_pshufb (shift, shift, 0))
#define BIGSIZE 0x10000000

#else /* __iset__ <= 4 */

#define MAKE(i) __builtin_e2k_qppermb (qsp[-(i)], qsp[-(i) - 1], qshift)
#define PREPARE E2K_PREPARE_ALIGN128 (shift, qshift)
#define BIGSIZE 0x8000000

#endif /* __iset__ <= 4 */

/* ************************************************************************** */

rettype
memmove (a1, a2, len)
     a1const void *a1;
     a2const void *a2;
     size_t len;
{
    unsigned char * restrict ac = (unsigned char *) dst;
    unsigned char * restrict bc = (unsigned char *) src;
    long i, diff;

    /* непересекающиеся блоки или если dst < src, копируем вперед */
    if (((unsigned long int)ac - (unsigned long int)bc) >= len) /* unsigned compare! */
    {

#if __iset__ <= 4

        /* короткие блоки копируем простым циклом.
         * для корректной работы основного кода граница здесь должна
         * быть не меньше 8. */
        if (len < 24)
        {
            if (len > 0) ac[0] = bc[0];

#pragma loop count(12)
            for (i = 1; i < len; i++)
            {
                ac[i] = bc[i];
            }
            RETURN (dst);
        }

        /* копируем невыровненную часть первого блока */
        diff = (- (size_t) ac) & 7;
        len -= diff;

        if (diff & 1) {
            *ac++ = *bc++;
        }
        if (diff & 2) {
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
            bc += 2;
            ac += 2;
        }
        if (diff & 4) {
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
            ((short *) ac)[1] = (bc[3] << 8) | bc[2];
            bc += 4;
            ac += 4;
        }

        /* копируем основную часть двойными словами */
        {
            unsigned long long * restrict al = (unsigned long long *) ac;
            unsigned long long * restrict bl = (unsigned long long *) ((long) bc & ~7);
            unsigned long long shift = (unsigned long long) (unsigned long) bc & 7;
            long ll = len >> 3;

            /* подготовка для копирования хвоста */
            ac += len & ~7;
            bc += len & ~7;

            if (len < 64 * 8) {       /* обрабатываем без apb */
                if (shift) { /* не выровненный src */
                    E2K_PREPARE_ALIGN (shift, shift);

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
                    for (i = 0; i < ll; i++)
                    {
                        E2K_ALIGN_DATA (bl[i], bl[i + 1], al[i], shift);
                    }
                }
                else { /* все выровнено */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
                    for (i = 0; i < ll; i++)
                    {
                        al[i] = bl[i];
                    }
                }
            }
            else if (ll < WC_LIMIT / 8) {       /* массив влезает в L2/L3 кэш */
                if (shift) { /* не выровненный src */
                    E2K_PREPARE_ALIGN (shift, shift);

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                    for (i = 0; i < ll; i++)
                    {
                        E2K_ALIGN_DATA (bl[i], bl[i + 1], al[i], shift);
                    }
                }
                else { /* все выровнено */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                    for (i = 0; i < ll; i++)
                    {
                        al[i] = bl[i];
                    }
                }
            }
            else { /* массив никуда не влезает */
                if (shift) { /* не выровненный src */
                    E2K_PREPARE_ALIGN (shift, shift);

#ifdef __ptr64__
                    int ii, isize;

#pragma loop count (1)
                    for (i = 0; i < ll; i += BIGSIZE) {
                        isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                        for (ii = 0; ii < isize; ii++)
                        {
                            unsigned long long res;
                            E2K_ALIGN_DATA (bl[ii], bl[ii + 1], res, shift);
                            __builtin_e2k_st_64s_nt (res, al + ii);
                        }
                        al += isize;
                        bl += isize;
                    }

#else  /* __ptr64__ */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                    for (i = 0; i < ll; i++)
                    {
                        unsigned long long res;
                        E2K_ALIGN_DATA (bl[i], bl[i + 1], res, shift);
                        __builtin_e2k_st_64s_nt (res, al + i);
                    }

#endif /* __ptr64__ */

                }
                else { /* все выровнено */

#ifdef __ptr64__
                    int ii, isize;

#pragma loop count (1)
                    for (i = 0; i < ll; i += BIGSIZE) {
                        isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                        for (ii = 0; ii < isize; ii++)
                        {
                            __builtin_e2k_st_64s_nt (bl[ii], al + ii);
                        }
                        al += isize;
                        bl += isize;
                    }

#else  /* __ptr64__ */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                    for (i = 0; i < ll; i++)
                    {
                        __builtin_e2k_st_64s_nt (bl[i], al + i);
                    }

#endif /* __ptr64__ */

                }
            }
        }

        /* копируем хвост */
        if (len & 4) {
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
            ((short *) ac)[1] = (bc[3] << 8) | bc[2];
            bc += 4;
            ac += 4;
        }
        if (len & 2) {
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
            bc += 2;
            ac += 2;
        }
        if (len & 1) {
            ac[0] = bc[0];
        }


#elif __iset__ <= 5

        __v2di * restrict qsp, qsrc0, qsrc1, qsrc, qshift;
        __v2di * restrict qdp = (__v2di *) ((size_t) ac & ~15);
        size_t len1;
        long shift, ll;

        diff = (size_t) ac & 15;
        int mask = 0xffff << diff;
        len1 = len - (16 - diff);
        shift = ((size_t) bc - diff) & 15;
        qsp = (__v2di *) (((size_t) bc - diff) & ~15);
        E2K_PREPARE_ALIGN128 (shift, qshift);

        qsrc0 = __builtin_e2k_ld_128_cleartag (qsp, 0);
        qsrc1 = __builtin_e2k_ld_128_cleartag (qsp, 16);
        E2K_ALIGN_DATA128 (qsrc0, qsrc1, qsrc, qshift);
        if (len <= (16 - diff))
            mask &= 0xffff >> (-len1);
        __builtin_e2k_pst_128 (qsrc, qdp, mask);
        if (len <= (16 - diff))
            RETURN (dst);

        diff = (-len1) & 15; /* вычисляем хвост */
        mask = 0xffff >> diff;
        ll = (len1 - (16 - diff)) >> 4;
        qsp++;
        qdp++;

        /* копируем основную часть квадро регистрами */
        if (ll <= 16) { /* до 256 байт копируем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
#pragma noprefetch
            for (i = 0; i < ll; i++) {
                E2K_ALIGN_DATA128 (qsp[i], qsp[i + 1], qdp[i], qshift);
            }

        } else {
            /* откручиваем 1 qp, чтобы не пересекать границу 32 байт */
            i = 0;
            if (((unsigned long) qsp & 31) == 0) {
                E2K_ALIGN_DATA128 (qsp[0], qsp[1], qdp[0], qshift);
                i = 1;
            }
            if (ll < WC_LIMIT / 16) { /* массив влезает в L3 кэш */

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                for (; i < ll; i++) {
                    E2K_ALIGN_DATA128 (qsp[i], qsp[i + 1], qdp[i], qshift);
                }

            } else { /* массив никуда не влезает */

#ifdef __ptr64__
                int ii, isize;

                qsp += i;
                qdp += i;
                ll -= i;

#pragma loop count (1)
                for (i = 0; i < ll; i += BIGSIZE) {
                    isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                    for (ii = 0; ii < isize; ii++)
                    {
                        E2K_ALIGN_DATA128 (qsp[ii], qsp[ii + 1], qsrc, qshift);
                        __builtin_e2k_st_128_nt (qsrc, qdp + ii);
                    }
                    qsp += isize;
                    qdp += isize;
                }
                ll = 0;

#else  /* __ptr64__ */

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                for (; i < ll; i++) {
                    E2K_ALIGN_DATA128 (qsp[i], qsp[i + 1], qsrc, qshift);
                    __builtin_e2k_st_128_nt (qsrc, qdp + i);
                }

#endif /* __ptr64__ */

            }
        }
        /* копируем хвост */
        qsrc0 = qsp[ll];
        qsrc1 = __builtin_e2k_ld_128_cleartag (qsp + ll + 1, 0);
        E2K_ALIGN_DATA128 (qsrc0, qsrc1, qsrc, qshift);
        __builtin_e2k_pst_128 (qsrc, qdp + ll, mask);


#else /* __iset__ > 5 */

        __v2di * restrict qdp = (__v2di *) ac;
        __v2di * restrict qsp = (__v2di *) bc;
        __v2di qsrc;
        long ll;

        if (len <= 16) {
            if (__builtin_expect (E2K_BYTES_FROM_ALIGN (qsp, 4096) > 4080, 0)) { /* closely to page border */
                /* Offsets 4081-4095 will be shifted back from the array last byte thus fit into page */
                qsrc = ((__v2di *) ((const char *) qsp - (16 - len)))[0];
                __builtin_pstoremas_128v (qsrc, (char *) qdp - (16 - len), 0xffff << (16 - len), __LCC_MAS_SPEC, __LCC_CHAN_ANY);
            }
            else {
                __builtin_pstoremas_128v (qsp[0], qdp, 0xffff >> (16 - len), __LCC_MAS_SPEC, __LCC_CHAN_ANY);
            }
            RETURN (dst);
        }

        diff = (-len) & 15; /* вычисляем хвост из diff байт (от 1 до 16) */
        ll = (len - (16 - diff)) >> 4;

        /* копируем основную часть квадро регистрами */
        if (ll <= 16) { /* до 256 байт копируем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
#pragma noprefetch
            for (i = 0; i < ll; i++) {
                qdp[i] = qsp[i];
            }

        } else if (ll < WC_LIMIT / 16) { /* массив влезает в L3 кэш */

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
            for (i = 0; i < ll; i++) {
                qdp[i] = qsp[i];
            }

        } else { /* массив никуда не влезает */

#ifdef __ptr64__
            int ii, isize;

#pragma loop count (1)
            for (i = 0; i < ll; i += BIGSIZE) {
                isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                for (ii = 0; ii < isize; ii++)
                {
                    __builtin_e2k_st_128_nt (qsp[ii], qdp + ii);
                }
                qsp += isize;
                qdp += isize;
            }
            ll = 0;

#else  /* __ptr64__ */

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
            for (i = 0; i < ll; i++) {
                __builtin_e2k_st_128_nt (qsp[i], qdp + i);
            }

#endif /* __ptr64__ */

        }
        /* копируем хвост */
        qsrc = ((__v2di *) ((const char *) (qsp + ll) - diff))[0];
        __builtin_e2k_pst_128 (qsrc, (char *) (qdp + ll) - diff, 0xffff << diff);

#endif /* __iset__ > 5 */

        RETURN (dst);
    }

    /* пересекающиеся блоки копируем задом наперед */
    {
        ac += len;
        bc += len;

#if __iset__ <= 4

        /* короткие блоки копируем простым циклом.
         * для корректной работы основного кода граница здесь должна
         * быть не меньше 8. */
        if (len < 24)
        {
            if (len > 0) ac[-1] = bc[-1];

#pragma loop count (12)
            for (i = 1; i < len; i++)
            {
                ac[-i - 1] = bc[-i - 1];
            }
            RETURN (dst);
        }

        /* копируем невыровненную часть первого блока */
        diff = (size_t) ac & 7;
        len -= diff;

        if (diff & 1) {
            *--ac = *--bc;
        }
        if (diff & 2) {
            bc -= 2;
            ac -= 2;
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
        }
        if (diff & 4) {
            bc -= 4;
            ac -= 4;
            ((short *) ac)[1] = (bc[3] << 8) | bc[2];
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
        }

        /* копируем основную часть двойными словами */
        {
            unsigned long long * restrict al = (unsigned long long *) ac;
            unsigned long long * restrict bl = (unsigned long long *) (((long) bc + 7) & ~7);
            unsigned long long shift = -(unsigned long long) (unsigned long) bc & 7;
            long ll = len >> 3;

            /* подготовка для копирования хвоста */
            ac -= len & ~7;
            bc -= len & ~7;
            i = 1;

            if (len < 64 * 8) {       /* обрабатываем без apb */
                if (shift) { /* не выровненный src */
                    PREPARE;
                    /* #68547 - откручиваем 1 дабл, чтобы не пересекать границу 32 байт */
                    if (((unsigned long) bl & 0x18) == 0x10) {
                        al[- 1] = MAKE (1);
                        i = 2;
                    }

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
                    for (; i <= ll; i++)
                    {
                        al[-i] = MAKE (i);
                    }
                }
                else { /* все выровнено */
                    /* #68547 - откручиваем 1 дабл, чтобы не пересекать границу 32 байт */
                    if (((unsigned long) bl & 0x18) == 0x8) {
                        al[-1] = bl[-1];
                        i = 2;
                    }

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
                    for (; i <= ll; i++)
                    {
                        al[-i] = bl[-i];
                    }
                }
            }
            else { /* массивы пересекаются, поэтому не используем запись мимо кеша */
                if (shift) { /* не выровненный src */
                    PREPARE;
                    /* #68547 - откручиваем 1 дабл, чтобы не пересекать границу 32 байт */
                    if (((unsigned long) bl & 0x18) == 0x10) {
                        al[- 1] = MAKE (1);
                        i = 2;
                    }

#ifdef __ptr64__
                    int ii, isize;

                    ll -= i - 1;
                    al -= i;
                    bl -= i;

#pragma loop count (1)
                    for (i = 0; i < ll; i += BIGSIZE) {
                        isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                        for (ii = 0; ii < isize; ii++)
                        {
                          al[-ii] = MAKE (ii);
                        }
                        al -= isize;
                        bl -= isize;
                    }

#else  /* __ptr64__ */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                    for (; i <= ll; i++)
                    {
                        al[-i] = MAKE (i);
                    }

#endif /* __ptr64__ */

                }
                else { /* все выровнено */

                    /* #68547 - откручиваем 1 дабл, чтобы не пересекать границу 32 байт */
                    if (((unsigned long) bl & 0x18) == 0x8) {
                        al[-1] = bl[-1];
                        i = 2;
                    }

#ifdef __ptr64__
                    int ii, isize;

                    ll -= i - 1;
                    al -= i;
                    bl -= i;

#pragma loop count (1)
                    for (i = 0; i < ll; i += BIGSIZE) {
                        isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                        for (ii = 0; ii < isize; ii++)
                        {
                            al[-ii] = bl[-ii];
                        }
                        al -= isize;
                        bl -= isize;
                    }

#else  /* __ptr64__ */

#pragma unroll (2) /* 1 такт */
#pragma loop count (1000)
                    for (; i <= ll; i++)
                    {
                        al[-i] = bl[-i];
                    }

#endif /* __ptr64__ */

                }
            }
        }

        /* копируем хвост */
        if (len & 4) {
            bc -= 4;
            ac -= 4;
            ((short *) ac)[1] = (bc[3] << 8) | bc[2];
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
        }
        if (len & 2) {
            bc -= 2;
            ac -= 2;
            ((short *) ac)[0] = (bc[1] << 8) | bc[0];
        }
        if (len & 1) {
            ac[-1] = bc[-1];
        }


#elif __iset__ <= 5

        __v2di * restrict qsp, qsrc0, qsrc1, qsrc, qshift;
        __v2di * restrict qdp = (__v2di *) ((size_t) ac & ~15);
        size_t len1;
        long shift, ll;

        diff = (size_t) ac & 15;
        if (diff == 0) {
            diff = 16;
            qdp--;
        }
        shift = ((size_t) bc - diff) & 15;
        qsp = (__v2di *) (((size_t) bc - diff) & ~15);
        int mask = 0xffff >> (16 - diff);
        len1 = len - diff;
        PREPARE;
        qsrc0 = __builtin_e2k_ld_128_cleartag (qsp, 16);
        qsrc1 = __builtin_e2k_ld_128_cleartag (qsp, 0);
        E2K_ALIGN_DATA128 (qsrc1, qsrc0, qsrc, qshift);
        if (len <= diff)
            mask &= 0xffff << (-len1);
        __builtin_e2k_pst_128 (qsrc, qdp, mask);
        if (len <= diff)
            RETURN (dst);

        diff = (-len1) & 15; /* вычисляем хвост */
        mask = 0xffff0000 >> (16 - diff);
        ll = (len1 - (16 - diff)) >> 4;

        /* копируем основную часть квадро регистрами */
        if (shift == 0) { /* выровненный src */
            if (ll <= 16) { /* до 256 байт копируем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
#pragma noprefetch
                for (i = 0; i < ll; i++) {
                    qdp[-i - 1] = qsp[-i - 1];
                }
            } else { /* массивы пересекаются, поэтому не используем запись мимо кеша */

                /* откручиваем 1 qp, чтобы не пересекать границу 32 байт */
                i = 0;
                if ((unsigned long) qsp & 31) {
                    qdp[- 1] = qsp[- 1];
                    i = 1;
                }

#ifdef __ptr64__
                int ii, isize;

                qsp -= i;
                qdp -= i;
                ll -= i;

#pragma loop count (1)
                for (i = 0; i < ll; i += BIGSIZE) {
                    isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                    for (ii = 0; ii < isize; ii++)
                    {
                        qdp[-ii - 1] = qsp[-ii - 1];
                    }
                    qsp -= isize;
                    qdp -= isize;
                }
                i = 0;

#else  /* __ptr64__ */

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                for (; i < ll; i++) {
                    qdp[-i - 1] = qsp[-i - 1];
                }

#endif /* __ptr64__ */

            }
            __builtin_e2k_pst_128 (qsp[-i - 1], qdp - i - 1, mask);
        }
        else { /* не выровненный src */
            if (ll <= 16) { /* до 256 байт копируем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
#pragma noprefetch
                for (i = 0; i < ll; i++) {
                    qdp[-i - 1] = MAKE (i);
                }
            } else { /* массивы пересекаются, поэтому не используем запись мимо кеша */

                /* откручиваем 1 qp, чтобы не пересекать границу 32 байт */
                i = 0;
                if ((unsigned long) qsp & 31) {
                    qdp[- 1] = MAKE (0);
                    i = 1;
                }

#ifdef __ptr64__
                int ii, isize;

                qsp -= i;
                qdp -= i;
                ll -= i;

#pragma loop count (1)
                for (i = 0; i < ll; i += BIGSIZE) {
                    isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                    for (ii = 0; ii < isize; ii++)
                    {
                        qdp[-ii - 1] = MAKE (ii);
                    }
                    qsp -= isize;
                    qdp -= isize;
                }
                i = 0;

#else  /* __ptr64__ */

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                for (; i < ll; i++) {
                    qdp[-i - 1] = MAKE (i);
                }

#endif /* __ptr64__ */

            }
            qsrc0 = qsp[-i];
            qsrc1 = __builtin_e2k_ld_128_cleartag (qsp - i - 1, 0);
            qsrc = __builtin_e2k_qppermb (qsrc0, qsrc1, qshift);
            __builtin_e2k_pst_128 (qsrc, qdp - i - 1, mask);
        }


#else /* __iset__ > 5 */

        __v2di * restrict qdp = (__v2di *) ac;
        __v2di * restrict qsp = (__v2di *) bc;
        __v2di qsrc;
        long ll;

        if (len <= 16) {
            if (__builtin_expect (E2K_BYTES_FROM_ALIGN (qsp, 4096) < 16, 0)) { /* closely to page border */
                /* Offsets 0-15 will be shifted forward to the array first byte thus fit into page */
                qsrc = ((__v2di *) ((const char *) qsp - len))[0];
                __builtin_pstoremas_128v (qsrc, (char *) qdp - len, 0xffff >> (16 - len), __LCC_MAS_SPEC, __LCC_CHAN_ANY);
            }
            else {
                __builtin_pstoremas_128v (qsp[-1], qdp - 1, 0xffff << (16 - len), __LCC_MAS_SPEC, __LCC_CHAN_ANY);
            }
            RETURN (dst);
        }

        diff = (-len) & 15; /* вычисляем хвост из diff байт (от 1 до 16) */
        ll = (len - (16 - diff)) >> 4;

        /* копируем основную часть квадро регистрами */
        if (ll <= 16) { /* до 256 байт копируем без apb */

#pragma unroll (1) /* 1 такт */
#pragma loop count (8)
#pragma noprefetch
            for (i = 0; i < ll; i++) {
                qdp[-i - 1] = qsp[-i - 1];
            }

        } else { /* массивы пересекаются, поэтому не используем запись мимо кеша */

#ifdef __ptr64__
            int ii, isize;

#pragma loop count (1)
            for (i = 0; i < ll; i += BIGSIZE) {
                isize = (ll - i) > BIGSIZE ? BIGSIZE : ll - i;

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
                for (ii = 0; ii < isize; ii++)
                {
                    qdp[-ii - 1] = qsp[-ii - 1];
                }
                qsp -= isize;
                qdp -= isize;
            }
            ll = 0;

#else  /* __ptr64__ */

#pragma unroll (4) /* 2 такта */
#pragma loop count (1000)
            for (i = 0; i < ll; i++) {
                qdp[-i - 1] = qsp[-i - 1];
            }

#endif /* __ptr64__ */

        }
        /* копируем хвост */
        qsrc = ((__v2di *) ((const char *) (qsp - ll - 1) + diff))[0];
        __builtin_e2k_pst_128 (qsrc, (char *) (qdp - ll - 1) + diff, 0xffff >> diff);

#endif /* __iset__ > 5 */

    }
    RETURN (dst);
}
#ifndef memmove
libc_hidden_builtin_def (memmove)
#endif
