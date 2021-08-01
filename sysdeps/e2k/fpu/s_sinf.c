/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  ЗНАЧЕНИЕ  ФУНКЦИИ  'СИНУС'.  %
%  АРГУМЕНТ  В  РАДИАНАХ.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA8  9.05183071609456021563e-04
#define KA6 -2.08608471166298886810e-02
#define KA4  2.53669325182559742429e-01
#define KA2 -1.23370054671140197041e+00
#define KA9  1.58206303556477327304e-04
#define KA7 -4.68126625672624547281e-03
#define KA5  7.96925827256306198148e-02
#define KA3 -6.45964096146795350000e-01
#define KA1  1.57079632678809990359e+00
#define LKA8 0x3f4da939d840136bLL
#define LKA6 0xbf955c8bc087f7d0LL
#define LKA4 0x3fd03c1e43ea1435LL
#define LKA2 0xbff3bd3cc8d2ecb7LL
#define LKA9 0x3f24bc85ccb213adLL
#define LKA7 0xbf732ca9d79fc665LL
#define LKA5 0x3fb466bbac8a54e8LL
#define LKA3 0xbfe4abbce56ae6e6LL
#define LKA1 0x3ff921fb5443b587LL

#define KB9  2.7249925803059791506e-06
#define KB7 -1.9840086735384846383e-04
#define KB5  8.3333318747102081564e-03
#define KB3 -1.6666666663855289610e-01
#define LKB9 0x3ec6dbe0838a06e9LL
#define LKB7 0xbf2a013a793b98ceLL
#define LKB5 0x3f811110def2e1c9LL
#define LKB3 0xbfc555555545e0acLL

FL __sinf (FL X)
{
    _type_float_bits arg;
    _type_double_bits darg;
    DB y, yv2, yv4;
    long k, ind;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv3, lyv4, lres, lres1;
#endif /* __iset__ < 6 */

    if (X == 0) return X; /* for X = -0.0 only */

    arg.value = ffabs (X);

    if (arg.int0 <= 0x3f490fdb) {    /* pi / 4 */
        y = (DB) X;
        yv2 = y * y;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
        lyv2.value = yv2;
        lyv3.value = y * yv2;
        lres.value = y;
        lres1.llong = __builtin_e2k_fmad (LKB5, lyv2.llong, LKB3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
        lyv3.value *= yv4; /* x^7 */
        lres1.llong = __builtin_e2k_fmad (LKB9, lyv2.llong, LKB7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
        return (FL) lres.value;
#else /* __iset__ < 6 */
        return (FL) (y + KB5 * y * yv2 * (KB3/KB5 + yv2) + KB9 * y * yv2 * yv4 * (KB7/KB9 + yv2));
#endif /* __iset__ < 6 */
    }

    if (unlikely (arg.int0 > 0x43c90fdb)) {     /* 2^7*pi */
        y = __rem_pio2f (arg.value, &ind);
        k = ind;
        /* if (k & 8) goto label; */   /* X = Inf or NaN */
    }
    else {
        y = K2ONPI * (DB) arg.value;
#if __iset__ <= 2
        darg.value = y + DVAIN52;
        y -= darg.value - DVAIN52;
        k = darg.twolong.int1;
#else /*  __iset__ <= 2 */
#pragma asm_inline
        __asm ("fdtoifd 0x0,%1,%0" : "=r" (darg.value): "r" (y)); /* rc=0 - to nearest */
        y -= darg.value;
        k = (int) darg.value;
#endif /*  __iset__ <= 2 */
    }
    yv2 = y * y;
    yv4 = yv2 * yv2;
    arg.value = X;

#if __iset__ >= 6
    lyv2.value = yv2;
    if (k & 1) {
        lyv4.value = yv4;
        lres.llong = __builtin_e2k_fmad (LKA2, lyv2.llong, 0x3ff0000000000000LL);
        lres.llong = __builtin_e2k_fmad (LKA4, lyv4.llong, lres.llong);
        y = lres.value;
        lres.llong = __builtin_e2k_fmsd (LKA8 ^ 0x8000000000000000LL, lyv2.llong, LKA6);
        yv4 = yv2 * yv4 * lres.value;
    }
    else {
        lyv3.value = y * yv2;
        lres.value = KA1 * y;
        lres1.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
        y = lres.value;
        lres.llong = __builtin_e2k_fmsd (LKA9 ^ 0x8000000000000000LL, lyv2.llong, LKA7);
        yv4 = lyv3.value * yv4 * lres.value;
    }
#else /* __iset__ < 6 */
    if (k & 1) {
        y = 1.0 + KA2 * yv2 + KA4 * yv4;
        yv4 = yv2 * yv4 * (-KA6 - KA8 * yv2);
    }
    else {
        yv4 = -KA9 * y * yv2 * yv4 * (KA7/KA9 + yv2);
        y = KA1 * y + KA5 * y * yv2 * (KA3/KA5 + yv2);
    }
#endif /* __iset__ < 6 */
    k ^= arg.int0 >> 30;
    /*label:*/
    return (FL) ((k & 2) ? yv4 - y : y - yv4);
}

weak_alias (__sinf, sinf)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  ПРОГРАММА  ПРИВЕДЕНИЯ  АРГУМЕНТА  К  ДИАПАЗОНУ        %
%              ОТ -PI/4  ДО  PI/4.                       %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#if __iset__ <= 2

#define TRUNC(__x) __trunc(__x)

#else /*  __iset__ <= 2 */

#define TRUNC(__x) ({                                      \
    DB __res;                                              \
    _Pragma ("asm_inline")                                 \
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (__res): "r" (__x)); \
    __res; })

#endif /*  __iset__ <= 2 */

static int ipio2_inf_f[] =
 {0xA2F983, 0x6E4E44, 0x1529FC, 0x2757D1, 0xF534DD, 0xC0DB62,
  0x95993C, 0x439041, 0xFE5163, 0xABDEBB, 0xC561B7, 0x246E3A
 };

DB __rem_pio2f (FL X, long *NSEG)
 /*
 /   X - исходный аргумент (должен быть положительным числом).
 /
 /   NSEG - указатель на целое, содержащее 3 младших разряда
 /         целого числа, ближайшего к значению [x*pi/2].
 /
 /   Функция возвращает аргумент, приведенный к диапазону
 /  -pi/4 до pi/4 и умноженному на 2/pi, в формате double.
 /
*/
{
#define one 1.0
#define two24 16777216.0
#define twon24 5.9604644775390625e-08

 _type_double_bits z;
 DB fw, q[10], tx;
 int e0, jz, jv, carry, n, iq[10], i, j, k, q0, ih, *ipio2_f;

 z.value=(DB) X;
 e0=exponentabs(z)-1046;
 if (e0==1001) {
     if (X == X) /* X is inf */
         __set_errno (EDOM);
     *NSEG=8;
     return z.value-z.value; /* NaN */
 }

 ipio2_f = ipio2_inf_f;

 z.number.exponent=1046;
 tx = (DB)((int)(z.value));

     /* determine jv,q0, note that 3>q0 */
 jv = (e0-3)/24; /*if (jv<0) jv=0;*/
 q0 =  e0-24-24*jv;

     /* compute q[0],q[1],...q[4] */
 j = jv;
 for(i=0;i<=2;i++,j++) q[i]=tx*(DB) ipio2_f[j];
 jz=2;

recompute:
    /* distill q[] into iq[] reversingly */
 for(i=0,j=jz,z.value=q[jz];j>0;i++)
   {fw=(DB)((int)(twon24* z.value));
    iq[i] = (int)(z.value-two24*fw);
    z.value = q[--j]+fw;
   }

    /* compute n */
 z.value=__scalbn(z.value,q0);   /* actual value of z */
 z.value -= 8.0*TRUNC(z.value*0.125); /* trim off integer >= 8 */
 n  = (int) z.value;
 z.value -= (DB)n;
 ih = 0;
 if(q0>0)       /* need iq[jz-1] to determine n */
   {i  = (iq[jz-1]>>(24-q0)); n += i;
    iq[jz-1] -= i<<(24-q0);
    ih = iq[jz-1]>>(23-q0);
   }
 else if(q0==0) ih = iq[jz-1]>>23;
      else if(z.value>=0.5) ih=2;

 if(ih>0)       /* q > 0.5 */
   {n++; carry = 0;
    for(i=0;i<jz ;i++)  /* compute 1-q */
      {j = iq[i];
       if(carry==0)
         {if(j!=0) {carry = 1; iq[i] = 0x1000000-j;}
         }
       else  iq[i] = 0xffffff - j;
      }
        /* rare case: chance is 1 in 12 */
    if (q0==1) iq[jz-1] &= 0x7fffff;
    else
      if (q0==2) iq[jz-1] &= 0x3fffff;
    if(ih==2)
      {z.value = one - z.value;
       if (carry!=0) z.value -= __scalbn(one,q0);
      }
   }

    /* check if recomputation is needed */
 if(z.value==0.0)
   {j = 0;
    for (i=jz-1;i>=2;i--) j |= iq[i];
    if(j==0)  /* need recomputation */
      {for(k=1;iq[2-k]==0;k++);   /* k = no. of terms needed */
       for(i=jz+1;i<=jz+k;i++)    /* add q[jz+1] to q[jz+k] */
         q[i] = tx*(DB) ipio2_f[jv+i];
       jz += k;
       goto recompute;
      }
   }

    /* chop off zero terms */
 if(z.value==0.0)
   {jz--; q0 -= 24;
    while(iq[jz]==0) { jz--; q0-=24;}
   }
 else { /* break z into 24-bit if necessary */
       z.value = __scalbn(z.value,-q0);
       if(z.value>=two24)
         {fw = (DB)((int)(twon24*z.value));
          iq[jz] = (int)(z.value-two24*fw);
          jz++; q0 += 24;
          iq[jz] = (int) fw;
         }
       else iq[jz] = (int) z.value ;
      }

    /* convert integer "bit" chunk to floating-point value */
 fw = __scalbn(one,q0);
 for(i=jz;i>=0;i--)
   {q[i] = fw*(DB)iq[i]; fw*=twon24;}

     /* compress q[] into y[] */
 fw = 0.0;
 for (i=jz;i>=0;i--) fw += q[i];
 fw = (ih==0) ? fw: -fw;
 *NSEG=n&7;
 return fw;
}
