/*
 * Copyright 08/26/02 Sun Microsystems, Inc.  All Rights Reserved.
 */

/* __k_sin_(double x[])
 *
 * Accuracy Table look-up algorithm by K.C. Ng, May, 1995.
 * Assume |x| <~ pi/4.
 * Return sin(x[0]+x[1])
 *
 * Updated 8/12/2002 to force f__sincos_, f__sin_, and f__cos_ agree.
 *
 * Algorithm: see f__sincos_.c
 */

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
/*
 *		    2	  4     6     8        -55.86
 * |cos(x) - (1+q1*x +q2*x +q3*x +q4*x  )| <= 2        for |x|<=0.1640625 (10.5/64)
 */
/* Q1  	= */ -0.5,
/* Q2  	= */  4.166666666500350703680945520860748617445e-0002,
/* Q3  	= */ -1.388888596436972210694266290577848696006e-0003,
/* Q4  	= */  2.478563078858589473679519517892953492192e-0005,
};

#define	ONE	sc[0]
#define	NONE	sc[1]
#define	PP1	sc[2]
#define	PP2	sc[3]
#define	P1	sc[4]
#define	P2	sc[5]
#define	P3	sc[6]
#define	P4	sc[7]
#define	QQ1	sc[8]
#define	QQ2	sc[9]
#define	Q1	sc[10]
#define	Q2	sc[11]
#define	Q3	sc[12]
#define	Q4	sc[13]

extern const double _TBL_sin[], _TBL_cos[], _TBL_sincosx[];

double
__k_sin_(double y[]) {
	double z, x, w, s, v, p, q;
	int i, j, n, hx, ix;

	x = y[0];
	hx = HI_WORD(x);
	ix = hx & ~0x80000000;

	if (ix <= 0x3fc50000) {	/* |x| < 10.5/64 = 0.164062500 */
		if (ix < 0x3e400000)
			if ((int) x == 0)
				return x + y[1];	/* |x| < 2**-27 */
		z = x * x;
		if (ix < 0x3f800000)
			p = (x * z) * (PP1 + z * PP2) + y[1];
							/* |x| < 0.008  */
		else
			p = (x * z) * ((P1 + z * P2) + (z * z) * (P3 +
				z * P4)) + y[1];
		return x + p;
	}
	else {			/* for 0.164062500 < x < ~pi/4 */
		n = ix >> 20;
		i = (((ix >> 12) & 0xff) | 0x100) >> (0x401 - n);
		j = i - 10;
		if (hx < 0)
			v = -y[1] - (_TBL_sincosx[j] + x);
		else
			v = y[1] - (_TBL_sincosx[j] - x);
		s = v * v;
		w = _TBL_sin[j];
		z = _TBL_cos[j];
		p = s * (PP1 + s * PP2);
		q = s * (QQ1 + s * QQ2);
		p = v + v * p;
		s = w * q + z * p;
		return hx >= 0 ? w + s : -(w + s);
	}
}
