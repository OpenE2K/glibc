/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  ?????????  ?????????  ???????????  ????????  ????? ?. %
%  ????????????  ?????????  ?????????  ????????  ? <= 0. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define LN2 0.6931471805599453094
#define KA3 6.66690704586487441045e-01

FL __logf (FL X)
{
    _type_float_bits arg;
    _type_double_bits y0;
    DB y, yp;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    yp = y0.value = (DB) X;
    if (X != X || arg.int0 >= 0x7f800000)    /* X = NaN ??? +Inf */
        return X + X;
    if (X <= 0)
        return (FL) SVID_libm_err (yp, yp, 116 + (X < 0));

    y0.llong = __builtin_e2k_paddd (y0.llong, 0x400000000000LL);
    y0.llong = __builtin_e2k_pandd (y0.llong, 0x7fff800000000000LL);
    y = (yp - y0.value) / (yp + y0.value);
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = y * y;
    lres.value = LN2 * (DB) ((y0.llong >> 52) - 1023) + constnln[(y0.llong >> 47) & 0x1f];
    lres.llong = __builtin_e2k_fmad (ly.llong, 0x4000000000000000LL, lres.llong);
    lres1.value = KA3 * y;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (y + y + (LN2 * (DB) ((y0.llong >> 52) - 1023)
        + constnln[(y0.llong >> 47) & 0x1f]) +  KA3 * y * (y * y));
#endif /* __iset__ < 6 */
}

strong_alias (__logf, __logf_finite)
weak_alias (__logf, logf)
weak_alias (__logf, __ieee754_logf)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   ?????????  ???  ???????   LOGF.                      %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
const DB constnln[]={
 0.0000000000000000000e+00,     /* LOG (1+ 0/32) */
 3.0771658666753687328e-02,     /* LOG (1+ 1/32) */
 6.0624621816434839938e-02,     /* LOG (1+ 2/32) */
 8.9612158689687138047e-02,     /* LOG (1+ 3/32) */
 1.1778303565638345574e-01,     /* LOG (1+ 4/32) */
 1.4518200984449788904e-01,     /* LOG (1+ 5/32) */
 1.7185025692665922836e-01,     /* LOG (1+ 6/32) */
 1.9782574332991986754e-01,     /* LOG (1+ 7/32) */
 2.2314355131420976486e-01,     /* LOG (1+ 8/32) */
 2.4783616390458126921e-01,     /* LOG (1+ 9/32) */
 2.7193371548364175805e-01,     /* LOG (1+10/32) */
 2.9546421289383589803e-01,     /* LOG (1+11/32) */
 3.1845373111853458870e-01,     /* LOG (1+12/32) */
 3.4092658697059319284e-01,     /* LOG (1+13/32) */
 3.6290549368936847463e-01,     /* LOG (1+14/32) */
 3.8441169891033205586e-01,     /* LOG (1+15/32) */
 4.0546510810816438486e-01,     /* LOG (1+16/32) */
 4.2608439531090008812e-01,     /* LOG (1+17/32) */
 4.4628710262841952972e-01,     /* LOG (1+18/32) */
 4.6608972992459923868e-01,     /* LOG (1+19/32) */
 4.8550781578170082442e-01,     /* LOG (1+20/32) */
 5.0455601075239531195e-01,     /* LOG (1+21/32) */
 5.2324814376454786835e-01,     /* LOG (1+22/32) */
 5.4159728243274440906e-01,     /* LOG (1+23/32) */
 5.5961578793542265942e-01,     /* LOG (1+24/32) */
 5.7731536503482361322e-01,     /* LOG (1+25/32) */
 5.9470710774669277576e-01,     /* LOG (1+26/32) */
 6.1180154110599294093e-01,     /* LOG (1+27/32) */
 6.2860865942237409421e-01,     /* LOG (1+28/32) */
 6.4513796137358470073e-01,     /* LOG (1+29/32) */
 6.6139848224536501586e-01,     /* LOG (1+30/32) */
 6.7739882359180614291e-01};    /* LOG (1+31/32) */
