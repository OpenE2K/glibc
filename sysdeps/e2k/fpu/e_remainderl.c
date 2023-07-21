/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ���������  ���������  ������������  �������  �����  %
%  X  ��  ������  Y.  ���������  ���������  � ���������  %
%  �� -Y/2 �� +Y/2.  ������������  ���������  ���������  %
%  ��������  Y = 0  ���  |X| = Inf.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

//#define TRUNC 6.77626357803440271255e-20L /* 1.25 / 2 ^ 64 */

LD __remainderl (LD X, LD Y)
{
    _type_long_double_bits absx, absy, ldz;
    LD hy, ly, recipy, half_y;
    LI ix, iy, sign, need_correct = 0;

    ldz.value = X;
    sign = ldz.twolong.short0 & 0x8000;
    ix = ldz.twolong.short0 & 0x7fff;

    absx.value = ldfabs (X);
    absy.value = ldfabs (Y);
    iy = absy.twolong.short0;

    if (unlikely (X != X || Y != Y))  /* X = NaN ��� Y = NaN */
        return (X * Y) / (X * Y);
    if (absx.value < absy.value * 0.5)
        return X;

    if (unlikely (Y == 0 || ix == 0x7fff)) /* Y = 0 ��� X = Inf */
        return SVID_libm_err (X, Y, 228);

    absx.value *= 0.5; /* ���������� 2 ���� �������� */
    half_y = 0.5 * absy.value;

    if (iy == 0) { /* Y - ����������������� */
        absy.value *= DVAIN63;
        iy = absy.twolong.short0;
        absx.value = __fmodl (absx.value, absy.value);
        absx.value *= DVAIN63;
        ix = absx.twolong.short0;
        need_correct = 1;
    }
    else {
        if (ix > (16382 + iy)) { /* ������� X/Y ����� ������������� */
            absx.value = __fmodl (absx.value, absy.value * DVAIN16382); /* 2^16382 */
            ix = absx.twolong.short0;
        }
    }
    recipy = 1 / absy.value;

    ldz.value = absy.value;
    ldz.twolong.long1 &= 0xffffffff00000000LL; /* ����� �� 2 �������� �� 32 ������� */
    ly = absy.value - ldz.value;
    hy = ldz.value;

#pragma loop count (10)
    while (ix > (31 + iy)) {
        ldz.value = absx.value * recipy; /* �������� ������� */
//        ldz.value -= ldz.value * TRUNC; /* ��������� �� 1 ulp */
        ldz.twolong.long1 &= 0xffffffff00000000LL; /* 32 ������� ��������� */
        absx.value -= ldz.value * hy; /* �������� �������, ���������� �� ������� �������� �������� */
        absx.value -= ldz.value * ly; /* �������� �������, ���������� �� ������� �������� �������� */
        ix = absx.twolong.short0;
    }

    ldz.value = (LD) (LL) (absx.value * recipy); /* �������� ������� � ���������� ��� �������������� � ����� */
    absx.value -= ldz.value * hy; /* �������� �������, ���������� �� ������� �������� �������� */
    absx.value -= ldz.value * ly; /* �������� �������, ���������� �� ������� �������� �������� */

    absx.value *= 2; /* �������������� �������� */
    if (absx.value > half_y) { /* �������� ����� ��������� */
        absx.value -= absy.value;
        if (absx.value >= half_y)
            absx.value -= absy.value;
    }
    if (need_correct) { /* ��� ��������� X � Y */
        absx.value *= (1 / DVAIN63);
    }
    absx.twolong.short0 ^= sign; /* ��������� ���� ���������� */
    return absx.value;
}

strong_alias (__remainderl, __remainderl_finite)
weak_alias (__remainderl, remainderl)
weak_alias (__remainderl, __ieee754_remainderl)
strong_alias (__remainderl, __dreml)
weak_alias (__remainderl, dreml)
