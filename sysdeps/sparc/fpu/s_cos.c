/*
 * Copyright 08/26/02 Sun Microsystems, Inc.  All Rights Reserved.
 */

/* cos(x)
 * Accuracy Table look-up algorithm by K.C. Ng, May, 1995.
 * Modified to agree with f_sincos.
 *
 * Algorithm: see f_sincos.c
 */

#include <errno.h>
#include <math.h>
#include <math_private.h>

#define HI_WORD(x) *(int*)&x
#define LO_WORD(x) *(1+(int*)&x)

static const double sc[] = {
/* ONE	= */  1.0,
/* NONE	= */ -1.0,
/*		     3	    5        -58.79
 * |sin(x) - (x+pp1*x +pp2*x  )| <= 2        for |x|< 0.008
 */
/* PP1	= */ -0.166666666666316558867252052378889521480627858683055567,
/* PP2	= */   .008333315652997472323564894248466758248475374977974017927,
/*
 * |(sin(x) - (x+p1*x^3+...+p4*x^9)|
 * |------------------------------ | <= 2^-57.63 for |x|<0.1953125
 * |                 x             |
 */
/* P1  	= */ -1.666666666666629669805215138920301589656e-0001,
/* P2  	= */  8.333333332390951295683993455280336376663e-0003,
/* P3  	= */ -1.984126237997976692791551778230098403960e-0004,
/* P4  	= */  2.753403624854277237649987622848330351110e-0006,
/*
 *		     2	    4        -55.99
 * |cos(x) - (1+qq1*x +qq2*x  )| <= 2        for |x|<=0.008 (0x3f80624d)
 */
/* QQ1	= */ -0.4999999999975492381842911981948418542742729,
/* QQ2	= */  0.041666542904352059294545209158357640398771740,
/* Q1  	= */ -0.5,
/* Q2  	= */  4.166666666500350703680945520860748617445e-0002,
/* Q3  	= */ -1.388888596436972210694266290577848696006e-0003,
/* Q4  	= */  2.478563078858589473679519517892953492192e-0005,
/* PIO2_H    = */  1.570796326794896557999,
/* PIO2_L    = */  6.123233995736765886130e-17,
/* PIO2_L0   = */  6.123233995727922165564e-17,
/* PIO2_L1   = */  8.843720566135701120255e-29,
/* PI3O2_H   = */  4.712388980384689673997,
/* PI3O2_L   = */  1.836970198721029765839e-16,
/* PI3O2_L0  = */  1.836970198720396133587e-16,
/* PI3O2_L1  = */  6.336322524749201142226e-29,
/* PI5O2_H   = */  7.853981633974482789995,
/* PI5O2_L   = */  3.061616997868382943065e-16,
/* PI5O2_L0  = */  3.061616997861941598865e-16,
/* PI5O2_L1  = */  6.441344200433640781982e-28,
};

#define	ONE		sc[0]
#define	PP1		sc[2]
#define	PP2		sc[3]
#define	P1		sc[4]
#define	P2		sc[5]
#define	P3		sc[6]
#define	P4		sc[7]
#define	QQ1		sc[8]
#define	QQ2		sc[9]
#define	Q1		sc[10]
#define	Q2		sc[11]
#define	Q3		sc[12]
#define	Q4		sc[13]
#define	PIO2_H		sc[14]
#define	PIO2_L		sc[15]
#define	PIO2_L0		sc[16]
#define	PIO2_L1		sc[17]
#define	PI3O2_H		sc[18]
#define	PI3O2_L		sc[19]
#define	PI3O2_L0	sc[20]
#define	PI3O2_L1	sc[21]
#define	PI5O2_H		sc[22]
#define	PI5O2_L		sc[23]
#define	PI5O2_L0	sc[24]
#define	PI5O2_L1	sc[25]

extern const double _TBL_sin[], _TBL_cos[], _TBL_sincosx[];
extern double __k_sin_(double y[]);
extern double __k_cos_(double y[]);
extern double __rem_pio2 (double x, int * nseg, double * lo);

double
__cos(double x) {
	double z, y[2], w, s, v, p, q;
	int i, j, n, hx, ix, lx;

	hx = HI_WORD(x);
	lx = LO_WORD(x);
	ix = hx & ~0x80000000;

	if (ix <= 0x3fc50000) {	/* |x| < 10.5/64 = 0.164062500 */
		if (ix < 0x3e400000) {	/* |x| < 2**-27 */
			if ((int) x == 0)
				return ONE;
		}
		else {
			z = x * x;
			if (ix < 0x3f800000)	/* |x| < 0.008 */
				w = z * (QQ1 + z * QQ2);
			else
				w = z * ((Q1 + z * Q2) + (z * z) * (Q3 +
					z * Q4));
			return ONE + w;
		}
	}
	/* for 0.164062500 < x < M, */
	n = ix >> 20;
	if (n < 0x402) {	/* x < 8 */
		i = (((ix >> 12) & 0xff) | 0x100) >> (0x401 - n);
		j = i - 10;
		x = fabs(x);
		v = x - _TBL_sincosx[j];
		if (((j - 81) ^ (j - 101)) < 0) {
			/* near pi/2, cos(pi/2-x)=sin(x) */
			p = PIO2_H - x;
			i = ix - 0x3ff921fb;
			x = p + PIO2_L;
			if ((i | ((lx - 0x54442D00) & 0xffffff00)) == 0) {
				/* very close to pi/2 */
				x = p + PIO2_L0;
				return x + PIO2_L1;
			}
			else {
				z = x * x;
				if (((ix - 0x3ff92000) >> 12) == 0) {
					/* |pi/2-x|<2**-8 */
					w = PIO2_L + (z * x) * (PP1 + z * PP2);
				}
				else
					w = PIO2_L + (z * x) * ((P1 + z * P2) +
						(z * z) * (P3 + z * P4));
				return p + w;
			}
		}
		s = v * v;
		if (((j - 282) ^ (j - 302)) < 0) {
			/* near 3/2pi, cos(x-3/2pi)=sin(x) */
			p = x - PI3O2_H;
			i = ix - 0x4012D97C;
			x = p - PI3O2_L;
			if ((i | ((lx - 0x7f332100) & 0xffffff00)) == 0) {
				/* very close to 3/2pi */
				x = p - PI3O2_L0;
				return x - PI3O2_L1;
			}
			else {
				z = x * x;
				if (((ix - 0x4012D800) >> 9) == 0) {
					/* |x-3/2pi|<2**-8 */
					w = (z * x) * (PP1 + z * PP2) - PI3O2_L;
				}
				else
					w = (z * x) * ((P1 + z * P2) + (z *
						z) * (P3 + z * P4)) - PI3O2_L;
				return p + w;
			}
		}
		if (((j - 483) ^ (j - 503)) < 0) {
			/* near 5pi/2, cos(5pi/2-x)=sin(x) */
			p = PI5O2_H - x;
			i = ix - 0x401F6A7A;
			x = p + PI5O2_L;
			if ((i | ((lx - 0x29553800) & 0xffffff00)) == 0) {
				/* very close to pi/2 */
				x = p + PI5O2_L0;
				return x + PI5O2_L1;
			}
			else {
				z = x * x;
				if (((ix - 0x401F6A7A) >> 7) == 0) {
					/* |pi/2-x|<2**-8 */
					w = PI5O2_L + (z * x) * (PP1 + z * PP2);
				}
				else
					w = PI5O2_L + (z * x) * ((P1 + z * P2) +
						(z * z) * (P3 + z * P4));
				return p + w;
			}
		}
		w = _TBL_sin[j];
		z = _TBL_cos[j];
		p = v + (v * s) * (PP1 + s * PP2);
		q = s * (QQ1 + s * QQ2);
		return z - (w * p - z * q);
	}
	else if (ix >= 0x7ff00000) {
		/* cos(Inf or NaN) is NaN */
                if (ix == 0x7ff00000 && lx == 0)
	            __set_errno (EDOM);
		return x - x;
	}
	else {
		/* argument reduction needed */
                y[0] = __rem_pio2 (x, &n, &y[1]);
		switch (n & 3) {
		case 0:
			return __k_cos_(y);
		case 1:
			return -__k_sin_(y);
		case 2:
			return -__k_cos_(y);
		case 3:
		default:
			return __k_sin_(y);
		}
	}
}

weak_alias (__cos, cos)
