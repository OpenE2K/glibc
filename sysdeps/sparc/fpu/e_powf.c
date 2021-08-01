/*
 * Copyright 02/25/97 Sun Microsystems, Inc.  All Rights Reserved.
 */
/* Copyright (C) 2011-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gmail.com>, 2011.

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

#include <math.h>
#include <math_private.h>
#include <math-svid-compat.h>

static const double
        ln2     =   6.93147180559945286227e-01, /* 0x3fe62e42, 0xfefa39ef */
        invln2  =   1.44269504088896338700e+00, /* 0x3ff71547, 0x652b82fe */
        dtwo    =   2.0,
        done    =   1.0,
        dhalf   =   0.5,
        dhuge   =   1e100,
        dtiny   =   1e-100,
        d32     =   32.0,
        d1_32   =   0.03125,
        A0      =   1.999999999813723303647511146995966439250e+0000,
        A1      =   6.666910817935858533770138657139665608610e-0001,
        t0      =   2.000000000004777489262405315073203746943e+0000,
        t1      =   1.666663408349926379873111932994250726307e-0001;

static const double S[] = {
         1.00000000000000000000e+00,    /* 3FF0000000000000 */
         1.02189714865411662714e+00,    /* 3FF059B0D3158574 */
         1.04427378242741375480e+00,    /* 3FF0B5586CF9890F */
         1.06714040067682369717e+00,    /* 3FF11301D0125B51 */
         1.09050773266525768967e+00,    /* 3FF172B83C7D517B */
         1.11438674259589243221e+00,    /* 3FF1D4873168B9AA */
         1.13878863475669156458e+00,    /* 3FF2387A6E756238 */
         1.16372485877757747552e+00,    /* 3FF29E9DF51FDEE1 */
         1.18920711500272102690e+00,    /* 3FF306FE0A31B715 */
         1.21524735998046895524e+00,    /* 3FF371A7373AA9CB */
         1.24185781207348400201e+00,    /* 3FF3DEA64C123422 */
         1.26905095719173321989e+00,    /* 3FF44E086061892D */
         1.29683955465100964055e+00,    /* 3FF4BFDAD5362A27 */
         1.32523664315974132322e+00,    /* 3FF5342B569D4F82 */
         1.35425554693689265129e+00,    /* 3FF5AB07DD485429 */
         1.38390988196383202258e+00,    /* 3FF6247EB03A5585 */
         1.41421356237309514547e+00,    /* 3FF6A09E667F3BCD */
         1.44518080697704665027e+00,    /* 3FF71F75E8EC5F74 */
         1.47682614593949934623e+00,    /* 3FF7A11473EB0187 */
         1.50916442759342284141e+00,    /* 3FF82589994CCE13 */
         1.54221082540794074411e+00,    /* 3FF8ACE5422AA0DB */
         1.57598084510788649659e+00,    /* 3FF93737B0CDC5E5 */
         1.61049033194925428347e+00,    /* 3FF9C49182A3F090 */
         1.64575547815396494578e+00,    /* 3FFA5503B23E255D */
         1.68179283050742900407e+00,    /* 3FFAE89F995AD3AD */
         1.71861929812247793414e+00,    /* 3FFB7F76F2FB5E47 */
         1.75625216037329945351e+00,    /* 3FFC199BDD85529C */
         1.79470907500310716820e+00,    /* 3FFCB720DCEF9069 */
         1.83400808640934243066e+00,    /* 3FFD5818DCFBA487 */
         1.87416763411029996256e+00,    /* 3FFDFC97337B9B5F */
         1.91520656139714740007e+00,    /* 3FFEA4AFA2A490DA */
         1.95714412417540017941e+00,    /* 3FFF50765B6E4540 */
};

static const double TBL[] = {
         0.00000000000000000e+00,
         3.07716586667536873e-02,
         6.06246218164348399e-02,
         8.96121586896871380e-02,
         1.17783035656383456e-01,
         1.45182009844497889e-01,
         1.71850256926659228e-01,
         1.97825743329919868e-01,
         2.23143551314209765e-01,
         2.47836163904581269e-01,
         2.71933715483641758e-01,
         2.95464212893835898e-01,
         3.18453731118534589e-01,
         3.40926586970593193e-01,
         3.62905493689368475e-01,
         3.84411698910332056e-01,
         4.05465108108164385e-01,
         4.26084395310900088e-01,
         4.46287102628419530e-01,
         4.66089729924599239e-01,
         4.85507815781700824e-01,
         5.04556010752395312e-01,
         5.23248143764547868e-01,
         5.41597282432744409e-01,
         5.59615787935422659e-01,
         5.77315365034823613e-01,
         5.94707107746692776e-01,
         6.11801541105992941e-01,
         6.28608659422374094e-01,
         6.45137961373584701e-01,
         6.61398482245365016e-01,
         6.77398823591806143e-01,
};

static const float
        one  = 1.0F,
        zero = 0.0F;

#define HIWORD  0
#define LOWWORD 1

float
__powf(float x, float y) {
        float fw, fz;
        int inf = 0x7f800000;
        int ix, iy, jx, jy, k, iw;

        ix = *(int *) &x;
        iy = *(int *) &y;
        jx = ix & 0x7fffffff;
        jy = iy & 0x7fffffff;

        if (jy == 0) {
                if (x != x) /* pow(NaN,0.0) */
                        return __kernel_standard_f (x, y, 142);
                if (jx == 0) /* pow(0.0,0.0) */
                        return __kernel_standard_f (x, y, 120);
                return one;     /* x**0 = 1 */
        }
        if (((inf - jx) | (inf - jy)) < 0) {
                if (ix == 0x3f800000)
                        return one;     /* 1**any = 1 */
                fz = x + y;     /* +-NaN return x+y */
                return fz;
        }

        /* determine if y is an odd int */
        if (ix < 0) {
                if (jy >= 0x4b800000) {
                        k = 0;  /* |y|>=2**24: y must be even */
                        iw = iy;
                }
                else {
                        k = (int) y;
                        fw = (float) k;
                        iw = *(int *) &fw;
                }               /* y is an odd int if k is odd and iw==iy */
        }

        /* special value of y */
        if ((jy & (~inf)) == 0) {
                if (jy == inf) {/* y is +-inf */
                        if (jx == 0x3f800000)
                                return one;     /* +-1**+-inf = 1 */
                        else if (jx > 0x3f800000) {     /* (|x|>1)**+,-inf =
                                                         * inf,0 */
                                if (iy > 0)
                                        fz = y;
                                else
                                        fz = zero;
                        }
                        else {  /* (|x|<1)**-,+inf = inf,0 */
                                if (iy < 0)
                                        fz = -y;
                                else
                                        fz = zero;
                        }
                        return fz;
                }
                if (jy == 0x3f800000) { /* y is  +-1 */
                        return (iy >= 0 ? x : (x != 0) ?
                                ( fz = one / x, (fabsf (fz) != INFINITY) ? fz :
                                  __kernel_standard_f (x, y, 121) ) : /* pow overflow */
                                  __kernel_standard_f (x, y, (ix == 0) ? 143 :123)); /* pow(+-0.0,negative) */
                }
                if (iy == 0x40000000) {
                        fz = x * x;     /* y is  2 */
                        if (fabsf (fz) == INFINITY && jx != 0x7f800000)
                                return __kernel_standard_f (x, y, 121); /* pow overflow */
                        else if (fz == 0  && jx != 0)
                                return __kernel_standard_f (x, y, 122); /* pow underflow */
                        return fz;
                }
                if (iy == 0x3f000000) {
                        if (ix > 0 && jx != inf)
                                return __builtin_sqrtf(x);      /* y is  0.5 */
                }
        }

        /* special value of x */
        if ((jx & (~inf)) == 0) {
                if (jx == inf || jx == 0 || jx == 0x3f800000) { /* x is +-0,+-inf,+-1 */
                        *(int *) &fz = jx;
                        if (jx == 0) { /* x = +-0 */
                                if (jy == 0) /* y = +-0 */
                                        return __kernel_standard_f (x, y, 120); /* pow(0.0,0.0) */
                                if (jy > 0x7f800000) /* y = nan */
                                        return (y * y); /* y is NaN; + -> * for Cheetah */
                                if (iy < 0) /* y < 0 */
                                        return (jy == 0x7f800000) ? (-y / x) :  /* y negative, return inf */
                                                __kernel_standard_f (x, y, (iw == iy && (k & 1) != 0) ? 123 : 143); /* pow(+-0,negative) */
                        }
                        if (iy < 0)
                                fz = one / fz;  /* fz = |x|**y */
                        if (ix < 0) {
                                if (jx == 0x3f800000 && iw != iy)
                                        return __kernel_standard_f (x, y, 124); /* (-1)**non-int */
                                else if (iw == iy && (k & 1) != 0)
                                        fz = -fz;       /* (x<0)**odd =
                                                         * -(|x|**odd) */
                        }
                        return fz;
                }
        }

        /* (x<0)**(non-int) is NaN */
        if (ix < 0 && iw != iy) {
                return __kernel_standard_f (x, y, 124);
        }

        /*
         * compute exp(y*log(|x|))
         * x = *(float *) &jx;
         * fz = (float) exp(((double) y) * log((double) x));
         */
        {
                double dx, dy, dz, ds;
                int *px = (int *) &dx, *pz = (int *) &dz, i, n, m;

                x = *(float *) &jx;
                dx = (double) x;

                /* compute log(x)/ln2 */
                i = px[HIWORD] + 0x4000;
                n = (i >> 20) - 0x3ff;
                pz[HIWORD] = i & 0xffff8000;
                pz[LOWWORD] = 0;
                ds = (dx - dz) / (dx + dz);
                i = (i >> 15) & 0x1f;
                dz = ds * ds;
                dy = invln2 * (TBL[i] + ds * (A0 + dz * A1));
                if (n == 0)
                        dz = (double) y * dy;
                else
                        dz = (double) y * (dy + (double) n);

                /* compute exp2(dz=y*ln(x)) */
                i = pz[HIWORD];
                if ((i & 0x7fffffff) >= 0x40640000) {   /* |z| >= 160.0 */
                        fz = (ix < 0 && (k & 1) == 1) ? -1 : 1;
                        if (i > 0)
                                return __kernel_standard_f (fz * x, y, 121); /* pow overflow */
                        else
                                return __kernel_standard_f (fz * x, y, 122); /* pow underflow */
                }
                n = d32 * dz + ((i > 0) ? dhalf : -dhalf);
                i = n & 0x1f;
                m = n >> 5;
                dy = ln2 * (dz - d1_32 * (double) n);
                dx = S[i] * (done - (dtwo * dy) / (dy * (done - dy * t1) - t0));
                if (m == 0)
                        fz = (float) dx;
                else {
                        px[HIWORD] += m << 20;
                        fz = (float) dx;
                }
        }

        /* end of computing exp(y*log(x)) */

        if (ix < 0 && (k & 1) == 1)
                fz = -fz;       /* (-ve)**(odd int) */

        return fz;
}

strong_alias (__powf, __powf_finite)
weak_alias (__powf, powf)
weak_alias (__powf, __ieee754_powf)
