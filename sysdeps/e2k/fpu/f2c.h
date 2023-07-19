/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/*****************************************************/
/*                                                   */
/*   ELBRUS LIBM                                     */
/*                                                   */
/*****************************************************/
/*             new types definition                  */
/*****************************************************/

#ifndef _F2C_H_
#define _F2C_H_

#include <math.h>
#include <math_private.h>
#include <math-svid-compat.h>

#define FL float
#define DB double
#define LD long double

typedef short int           SI;
typedef long int            LI;
typedef long long           LL;
typedef unsigned long long  ULL;

typedef union {
  LD value;
  struct {
    unsigned significand2 : 32;
    unsigned significand : 32;
    unsigned exponent : 15;
    unsigned sign : 1;
    unsigned unused0 : 16;
    unsigned unused1 : 32;
  } number;
  struct {
    LL long1; SI short0;
  } twolong;
/*****************************************************/
} _type_long_double_bits;

typedef union {
  DB value;
  LL llong;

  struct {
    unsigned significand2 : 32;
    unsigned significand : 20;
    unsigned exponent : 11;
    unsigned sign : 1;
  } number;

  struct {
    int int1, int0;
  } twolong;
} _type_double_bits;

typedef union {
  FL value;

  struct {
    unsigned significand : 23;
    unsigned exponent : 8;
    unsigned sign : 1;
  } number;

  int int0;
} _type_float_bits;

/*****************************************************/
/*                auxiliary functions                */
/*****************************************************/

#if defined (__LCC__) || defined (__GNUC__)

#define __inline_sqrtf(__xx)                                  \
  ({  float __rr = __xx;                                      \
_Pragma ("asm_inline")                                        \
  __asm ("fsqrts %0,%0" : "+r" (__rr));                       \
  __rr; })

#define __inline_sqrt(__xx)                                   \
  ({  double __rr = __xx, __tmp;                              \
_Pragma ("asm_inline")                                        \
  __asm ("fsqrtid %1, %0" : "=r" (__tmp) : "r" (__rr));       \
_Pragma ("asm_inline")                                        \
  __asm ("fsqrttd %0, %1, %0" : "+r" (__rr) : "r" (__tmp));   \
  __rr; })

#define __inline_sqrtl(__xx)                                  \
  ({  long double __rr = __xx, __tmp;                         \
_Pragma ("asm_inline")                                        \
  __asm ("fxsqrtixx %1, %0" : "=r" (__tmp) : "r" (__rr));     \
_Pragma ("asm_inline")                                        \
  __asm ("fxsqrtuxx %1, %0, %0" : "+r" (__tmp) : "r" (__rr)); \
_Pragma ("asm_inline")                                        \
  __asm ("fxsqrttxx %0, %1, %0" : "+r" (__rr) : "r" (__tmp)); \
  __rr; })

#define ffabs __builtin_fabsf
#define dfabs __builtin_fabs
#define ldfabs __builtin_fabsl
#define fsqrt __inline_sqrtf
#define dsqrt __inline_sqrt
#define ldsqrt __inline_sqrtl

#else

#define ffabs fabsf
#define dfabs fabs
#define ldfabs fabsl
#define fsqrt sqrtf
#define dsqrt sqrt
#define ldsqrt sqrtl

extern FL fsqrt (FL);
extern DB dsqrt (DB);
extern LD ldsqrt (LD);

#endif /* __LCC__ */

extern FL ffabs (FL);
extern DB dfabs (DB);
extern LD ldfabs (LD);

#define SVID_libm_err __kernel_standard
extern DB SVID_libm_err (DB, DB, int);
extern DB __rem_pio2 (DB, DB *, int *);
extern DB __rem_pio2f (FL, long *);
extern DB __rem_pio2l (LD, DB *, int *);
extern DB __scalbn (DB,int);
extern DB __trunc (DB);

/*****************************************************/
/*              common constant tables               */
/*****************************************************/

extern const LD
     constexpl[],         /* 2**(i/128), i=[0,127] */
     constexpl_lo[];      /* low part of 2**(i/128), i=[0,127] */

extern const DB
     constarctg_lo[],
     constln_lo[],
     constexp[],          /* 2**(i/256), i=[0,255] */
     constexp_lo[],       /* low part of 2**(i/256), i=[0,255] */
     constnarctg[],       /*  */
     constnexp[],         /*  */
     constnln[];          /*  */
extern const int
     constarctg[],        /*  */
     constln[];           /*  */

/*****************************************************/
/*               constants definition                */
/*****************************************************/

#define Inf_LD      __builtin_infl()
#define NaN_LD      __builtin_nanl("")
#define LD_MIN      3.36210314311209350626e-4932L /* normalized */
#define LD_MAX      1.18973149535723176502e+4932L
#define LD_MIN_UN   3.64519953188247460253e-4951L /* denormalized */
#define LD_MAX_UN   3.36210314311209350590e-4932L
#define LD_MAX_LOG  11356.5234062941439488L
#define LD_MIN_LOG -11398.8053843083006136L
#define DVAIN63     9.223372036854775808e+18L
#define DVAIN16382  2.97432873839307941271e+4931L
#define LD128ONLN2    184.664965233787316146L
#define LDLN2ON128_hi 5.41521234812591956143e-03L  /* 0xb17217f7d2000000, 0x3ff7 */
#define LDLN2ON128_lo -1.34683160356365896663e-15L /* 0xc21950d871319ff0, 0xbfcd */
#define LDPI        3.14159265358979323851L
#define LDPION2     1.57079632679489661926L

#define Inf         __builtin_inf()
#define NaN         __builtin_nan("")
#define DB_MIN      2.2250738585072014e-308 /* normalized */
#define DB_MAX      1.7976931348623157e+308
#define DB_MIN_UN   4.9406564584124654e-324 /* denormalized */
#define DB_MAX_UN   2.2250738585072009e-308
#define DB_MAX_LOG  709.7827128933839731
#define DB_MIN_LOG -745.1332191019412221
#define MAX_INT     0x7fffffff

#define PI 3.1415926535897931160e+00
#define PI_lo 1.2246467991473531772e-16
#define LPI_lo 0x3ca1a62633145c07LL
#define PION2 1.57079632679489655800e+00
#define LPION2 0x3ff921fb54442d18LL
#define PION2_lo 6.12323399573676603587e-17
#define LPION2_lo 0x3c91a62633145c07LL
#define DVAIN52 4.503599627370496e15
#define DVAIN1022 4.494232837155789769e307
#define PION512 6.135923089459538460e-03  /* 0-> 28 bits */
#define PION512_lo 6.208302645909451584e-11
#define K512ONPI 162.9746617261008339
#define KC4 4.16666503257176243222e-02
#define KC2 -4.99999999999961548163e-01
#define KS5 8.33333053202772436238e-03
#define KS3 -1.66666666666656779051e-01
#define KPION2_hi1 1.57079632673412561417e+00 /* first 33 bit of pi/2 */
#define KPION2_lo1 6.07710050650619224932e-11
#define KPION2_hi2 6.07710050630396597660e-11 /* second 33 bit of pi/2 */
#define KPION2_lo2 2.02226624879595063154e-21
#define K2IN54 5.551115123125782702e-17
#define K2IN110 7.703719777548943412e-34
#define MAX_SINH 710.4758600739439771
#define K256ONLN2    369.3299304675746271
#define KLN2ON256_hi 2.707606173771637259e-03
#define LKLN2ON256_hi 0x3f662e42fef00000LL
#define KLN2ON256_lo 2.906491058598592519e-13
#define LKLN2ON256_lo 0x3d5473de6af278edLL
#define LN2 0.6931471805599453094
#define KLN2_lo 1.9082149292705877e-10
#define KLN2_hi 0.6931471803691238165
#define K180ONPI 5.729577951308232286e+01
#define KPION180 1.745329251994329547e-02
#ifndef X_TLOSS
#define X_TLOSS 1.414847550405688e+16  /* pi*2^52 , def in <values.h> ? */
#endif

#define Inf_F       __builtin_inff()
#define NaN_F       __builtin_nanf("")
#define FL_MIN      1.17549435e-38F /* normalized */
#define FL_MAX      3.402823466E+38F
#define FL_MIN_UN   1.40129846e-45F /* denormalized */
#define FL_MAX_UN   1.17549421e-38F
#define FL_MAX_LOG  88.7228394F
#define FL_MIN_LOG -103.972076F

#define K1ONPI 3.183098861837906912e-01
#define K2ONPI 6.366197723675813824e-01
#define DVAIN23 8388608.0F
#define MAX_SINH_F 89.4159851F

/*****************************************************/
/*                 macros definition                 */
/*****************************************************/

#define exponent(x) ((x.llong>>52)&2047)
#define exponentabs(x) (x.llong>>52)

#define exponent_F(x) ((x.int0>>23)&255)
#define exponentabs_F(x) (x.int0>>23)

/*****************************************************/
/*              new macros definition                */
/*****************************************************/

#define unlikely(x)  __builtin_expect(!!(x), 0)

typedef union
{
    double db;
    long long ll;
    struct {
        int i1, i0;
    } two_int;

    struct {
        float f1, f0;
    } two_float;

    struct {
      unsigned char ch7, ch6, ch5, ch4, ch3, ch2, ch1, ch0;
    } eight_char;
} type_union_64f;

typedef union {
    int i0;
    float f0;
} type_union_32f;

#define HI(x)  (*(1 + (unsigned int *) &(x)))
#define LO(x)  (*(unsigned int *) &(x))

#endif
