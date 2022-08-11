/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  ГЛАВНОЕ  ЗНАЧЕНИЕ  ФУНКЦИИ   %
%  'АРККОСИНУС'.  НЕДОПУСТИМЫМ   ЗНАЧЕНИЕМ   АРГУМЕНТА   %
%  ЯВЛЯЕТСЯ  |Х| > 1.  РЕЗУЛЬТАТ  В  РАДИАНАХ.           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA16  3.2074126867121511208145e-02
#define KA15 -2.9288652492291338371496e-02
#define KA14  2.6492027340877369314240e-02
#define KA13 -2.7168811083038328128390e-03
#define KA12  8.7411030772738116956518e-03
#define KA11  6.8694448704992530617248e-03
#define KA10 8.4524060657465405568166e-03
#define KA9  9.7552753161635354625737e-03L
#define KA8  1.1552268573915126970151e-02L
#define KA7  1.3964819214172544970724e-02L
#define KA6  1.7352765309284953350076e-02L
#define KA5  2.2372159069960181287570e-02L
#define KA4  3.0381944444743147852574e-02L
#define KA3  4.4642857142854911058000e-02L
#define KA2  7.5000000000000006579088e-02L
#define KA1  1.6666666666666666666345e-01L

LD __acosl (LD X)
{
    _type_long_double_bits arg, rez;
    LD z, z1, s, p, zv2, zv4, zv8;

    arg.value = ldfabs (X);

    if ((arg.twolong.short0 & 0x7fff) >= 0x3fff) /* |x| >= 1 или NaN */
        return (arg.value > 1) ? SVID_libm_err (X, 0, 201)
            : LDPION2 - X * LDPION2;  /* NaN или ПИ/2*(sign(X)+1) */

    if (arg.value <= 0.5) { /* |x| <= 0.5 */
        z = X * X;
        zv2 = z * z;
        zv4 = zv2 * zv2;
        zv8 = zv4 * zv4;
        p = KA2 * z * (KA1 / KA2 + z) + KA3 * z * zv2 + zv4 * (KA4 +
            KA5 * z) + KA9 * zv2 * zv4 * (KA6 / KA9 + KA7 / KA9 * z +
            zv2 * (z + KA8 / KA9)) + KA13 * zv2 * (KA10 / KA13 + KA11 / KA13 * z +
            zv2 * (z + KA12 / KA13)) * zv8 + zv2 * zv4 * zv8 * (KA14 + KA15 * z + KA16 * zv2);
        p *= X;
        z = PION2;
        if (arg.value > 9.5367431640625e-7) { /* |x| > 2^-20 */
            rez.value = X;
            rez.twolong.long1 &= 0xffffffff00000000ULL;
            X -= rez.value;
            z -= rez.value;
        }
        return PION2_lo - X - p + z;
    }
    z = 1 - arg.value; /* acos(x) = 2 * asin (sqrt ((1 - x) / 2) */
    z1 = z + z;
    s = ldsqrt (z1);
    z *= 0.5;
    zv2 = z * z;
    zv4 = zv2 * zv2;
    zv8 = zv4 * zv4;
// Последняя часть полинома вычисляется последовательно для большей точности.
// Все равно есть запас времени до окончания sqrtx и fdivx.
    p = zv4 * (KA4 + KA5 * z) + KA9 * zv2 * zv4 * (KA6 / KA9 + KA7 / KA9 * z +
        zv2 * (z + KA8 / KA9)) + KA13 * zv2 * (KA10 / KA13 + KA11 / KA13 * z +
        zv2 * (z + KA12 / KA13)) * zv8 + zv2 * zv4 * zv8 * (KA14 + KA15 * z +
        KA16 * zv2) + ((KA3 * z + KA2) * z + KA1) * z;
    p *= s;
    rez.value = s;
    rez.twolong.long1 &= 0xffffffff00000000ULL;
    z = (z1 - rez.value * rez.value) / (s + rez.value);
    if (X < 0) {
        return PI_lo - p - z + (PI - rez.value);
    }
    else {
        return p + z + rez.value;
    }
}

strong_alias (__acosl, __acosl_finite)
weak_alias (__acosl, acosl)
weak_alias (__acosl, __ieee754_acosl)
