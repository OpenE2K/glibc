/*
 * Copyright 07/16/00 Sun Microsystems, Inc.  All Rights Reserved.
 */

/* tan(x)
 * Accuracy Table look-up algorithm by K.C. Ng, May, 1995. 
 * Updated on July 16, 2000.
 * 1. For |x| < 0.1640625,
 *	tan(x) = x + x^3*(T1+x^2*(T2+x^2*(T3+x^2*(T4+x^2*(T5+x^2*T6)))))
 *	       = x+(x*v)*(((T1+T2*v)+(v*v)*T3)+((v*v)*v)*((T4+v*T5)+(v*v)*T6));
 *      where v=x*x.
 * 2. Reduce x to x>0 by tan(-x)=-tan(x).
 * 3. For 0<= x < 8, let i = (64*x chopped)-10. Let d = x - a[i], where
 *    a[i] is a double that is close to (i+10.5)/64 and such that 
 *    sin(a[i]) and cos(a[i]) is close to a double (with error less 
 *    than 2**-8 ulp). Then
 *	tan(x) = sin(x)/cos(x), where
 *	cos(x) = cos(a[i]+d) = cos(a[i])cos(d) - sin(a[i])*sin(d)
 *	       = TBL_cos_a[i]*(1+QQ1*d^2+QQ2*d^4)-TBL_sin_a[i]*(d+PP1*d^3+PP2*d^5)
 *	       = TBL_cos_a[i] + 
 *		  (TBL_cos_a[i]*d^2*(QQ1+QQ2*d^2)-TBL_sin_a[i]*(d+PP1*d^3+PP2*d^5))
 *	sin(x) = sin(a[i]+d) = sin(a[i])cos(d) + cos(a[i])*sin(d)
 *	       = TBL_sin_a[i]*(1+QQ1*d^2+QQ2*d^4)+TBL_cos_a[i]*(d+PP1*d^3+PP2*d^5)
 *	       = TBL_sin_a[i] + 
 *		  (TBL_sin_a[i]*d^2*(QQ1+QQ2*d^2)+TBL_cos_a[i]*(d+PP1*d^3+PP2*d^5))
 *
 *    Note: for x near k*pi/2, one of sine or cosine is near zero and to
 *	    avoid loss of accuracy, we apply argument reduction on those
 *	    interval as follows:
 *		x in [91/64,111/64),   |x-1*pi/2|<.163579
 *			y = x - pi/2
 *			sin(x) =  cos(y)
 *			cos(x) = -sin(y)
 *			tan(x) = -1/tan(y)
 *		x in [191/64,211/64),  |x-2*pi/2|<.157218
 *			y = x - pi
 *			sin(x) = -sin(y)
 *			cos(x) = -cos(y)
 *			tan(x) = tan(y)
 *		x in [292/64,312/64),  |x-3*pi/2|<.162611
 *			y = x - 3*pi/2
 *			sin(x) =  cos(y)
 *			cos(x) = -sin(y)
 *			tan(x) = -1/tan(y)
 *		x in [392/64,412/64),  |x-4*pi/2|<.158185
 *			y = x - 2*pi
 *			sin(x) =  sin(y)
 *			cos(x) =  cos(y)
 *			tan(x) =  tan(y)
 *		x in [493/64,513/64),  |x-5*pi/2|<.161643
 *			y = x - 5*pi/2
 *			sin(x) =  cos(y)
 *			cos(x) = -sin(y)
 *			tan(x) = -1/tan(y)
 *          for |y| < 0.1640625, 
 *		sin(y) = y + y^3*(p1+y^2*(p2+y^2*(p3+y^2*p4)))
 *		cos(y) = 1 + y^2*(q1+y^2*(q2+y^2*(q3+y^2*q4)))
 *		tan(y) = y + y^3*(T1+y^2*(T2+y^2*(T3+y^2*(T4+y^2*(T5+y^2*T6)))))
 *	
 * 4. For x >= 8.0
 *
 * kernel function:
 *	__k_sincos_	... sine and cosine function on [-pi/4,pi/4]
 *	__rem_pio2	... argument reduction routine
 *
 * Method.
 *      Let S and C denote the sin and cos respectively on [-PI/4, +PI/4].
 *      1. Assume the argument x is reduced to y1+y2 = x-k*pi/2 in
 *	   [-pi/2 , +pi/2], and let n = k mod 4.
 *	2. Let S=S(y1+y2), C=C(y1+y2). Depending on n, we have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0	       S	   C		 S/C
 *	    1	       C	  -S		-C/S
 *	    2	      -S	  -C		 S/C
 *	    3	      -C	   S		-C/S
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *	TAN(x) returns tan(x) nearly rounded
 */

#include <errno.h>
#include <math.h>
#include <math_private.h>

#define __HI(x)  (*(unsigned *) &(x))
#define __LO(x)  (*(1 + (unsigned *) &(x)))

static double sc[] = {
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
/* PIO2_H    = */  1.570796326794896557999,
/* PIO2_L    = */  6.123233995736765886130e-17,
/* PIO2_L0   = */  6.123233995727922165564e-17,
/* PIO2_L1   = */  8.843720566135701120255e-29,
/* PI_H = */  3.1415926535897931159979634685,
/* PI_L    = */  1.22464679914735317722606593227425e-16,
/* PI_L0   = */  1.22464679914558443311283879205095e-16,
/* PI_L1   = */  1.768744113227140223300005233735517376e-28,
/* PI3O2_H   = */  4.712388980384689673997,
/* PI3O2_L   = */  1.836970198721029765839e-16,
/* PI3O2_L0  = */  1.836970198720396133587e-16,
/* PI3O2_L1  = */  6.336322524749201142226e-29,
/* PI2_H   = */  6.2831853071795862319959269370,
/* PI2_L   = */  2.44929359829470635445213186454850e-16,
/* PI2_L0  = */  2.44929359829116886622567758410190e-16,
/* PI2_L1  = */  3.537488226454280446600010467471034752e-28,
/* PI5O2_H   = */  7.853981633974482789995,
/* PI5O2_L   = */  3.061616997868382943065e-16,
/* PI5O2_L0  = */  3.061616997861941598865e-16,
/* PI5O2_L1  = */  6.441344200433640781982e-28,
/*
 * |(tan(x) - (x+T1*x^3+...+T6*x^13)|
 * |--------------------------------| <= 2^-58.74 for |x|<=0.1640625
 * |                 x              |
 */
/* T1 = */   3.33333333333328549634177997290625933704910946084e-0001,
/* T2 = */   1.33333333336782617293185759830667017686940106633e-0001,
/* T3 = */   5.39682531403696138113460968559482581229223452054e-0002,
/* T4 = */   2.18695798286962808388672705534647009540194567419e-0002,
/* T5 = */   8.85815351070410312529572837885115735871863084967e-0003,
/* T6 = */   3.73076034788169677120611811081147856562556419861e-0003,
};

#define ONE  	sc[0]
#define NONE  	sc[1]
#define PP1 	sc[2]
#define PP2 	sc[3]
#define P1 	sc[4]
#define P2 	sc[5]
#define P3 	sc[6]
#define P4 	sc[7]
#define QQ1 	sc[8]
#define QQ2 	sc[9]
#define Q1 	sc[10]
#define Q2 	sc[11]
#define Q3 	sc[12]
#define Q4 	sc[13]
#define PIO2_H 	sc[14]
#define PIO2_L 	sc[15]
#define PIO2_L0	sc[16]
#define PIO2_L1	sc[17]
#define PI_H 	sc[18]
#define PI_L 	sc[19]
#define PI_L0 	sc[20]
#define PI_L1 	sc[21]
#define PI3O2_H 	sc[22]
#define PI3O2_L 	sc[23]
#define PI3O2_L0 	sc[24]
#define PI3O2_L1 	sc[25]
#define PI2_H 	sc[26]
#define PI2_L 	sc[27]
#define PI2_L0 	sc[28]
#define PI2_L1 	sc[29]
#define PI5O2_H 	sc[30]
#define PI5O2_L 	sc[31]
#define PI5O2_L0 	sc[32]
#define PI5O2_L1 	sc[33]
#define T1 	sc[34]
#define T2 	sc[35]
#define T3 	sc[36]
#define T4 	sc[37]
#define T5 	sc[38]
#define T6 	sc[39]

extern const double _TBL_sin[], _TBL_cos[], _TBL_sincosx[];
extern double __k_sincos_(double y[], double *c);
extern double __rem_pio2 (double x, int * nseg, double * lo);

double 
__tan(double x) {
	double z,y[2],w,s,t,v,p,q;
	int i,j,n,hx,ix;
	unsigned lx;
	hx = __HI(x); 
	lx = __LO(x); ix = ((unsigned)(hx<<1))>>1;
	if(ix <= 0x3fc50000) {		/* |x| < 10.5/64 = 0.164062500 */
	    if (ix<0x3e400000)  		/* |x| < 2**-27 */
		{if((int)x==0) return x;}
	    else {
                v = x*x;
		return x+(x*v)*(((T1+T2*v)+(v*v)*T3)+((v*v)*v)*((T4+v*T5)+(v*v)*T6));
	    }
	}

    /* for 0.164062500 < x < 8, */
	n = ix>>20;
	if(n < 0x402) {	/* x < 8 */
	    i = (((ix>>12)&0xff)|0x100)>>(0x401-n);
	    j = i-10; 
	    x = fabs(x);
	    v = x-_TBL_sincosx[j];
	    s = v*v;
	    if(((i-91)^(i-111))<0) {		/* if(i>=91&&i<111) */
    		/* near pi/2, y = x-pi/2, cos(x)= -sin(y), sin(x)=cos(y) */
		p = x-PIO2_H;		
		i = ix-0x3ff921fb;
		x = p-PIO2_L;
		if((i|((lx>>8)-0x54442D))==0) {	/* very close to pi/2 */
		    t = p-PIO2_L0;
		    return (hx>=0)? ONE/(PIO2_L1-t): ONE/(t-PIO2_L1);
		} else {
		    v = x*x;
		    z = p-(PIO2_L0-(x*v)*(((T1+T2*v)+(v*v)*T3)+
				((v*v)*v)*((T4+v*T5)+(v*v)*T6)));
		    return (hx>=0)? NONE/z: ONE/z;
		}
	    }
	    if(((i-191)^(i-211))<0) {		/* if(i>=191&&i<211) */
    		/* near pi, y = x-pi, tan(x)=tan(y) */
		p = x-PI_H;		
		i = ix-0x400921fb;
		x = p-PI_L;
		if((i|((lx>>8)-0x54442D))==0) {/* very close to pi */
		    t = p-PI_L0;
		    return (hx>=0)? (t-PI_L1): (PI_L1-t);
		} else {
		    v = x*x;
		    z = p-(PI_L-(x*v)*(((T1+T2*v)+(v*v)*T3)+
				((v*v)*v)*((T4+v*T5)+(v*v)*T6)));
		    return (hx>=0)? z: -z;
		}
	    }
	    if(((i-292)^(i-312))<0) {		/* if(i>=292&&i<312) */
	    		/* near 3pi/2, y=x-3pi/2, sin(x)=cos(y),cos(x)=-sin(y) */
		p = x-PI3O2_H;		
		i = ix-0x4012D97C;
		x = p-PI3O2_L;
		if((i|((lx>>8)-0x7f332100))==0) {/* very close to 3/2pi */
		    x = p-PI3O2_L0;
		    return (hx>=0)? ONE/(PI3O2_L1-x):ONE/(x-PI3O2_L1);
		} else {
		    v = x*x;
		    z = p-(PI3O2_L-(x*v)*(((T1+T2*v)+(v*v)*T3)+
				((v*v)*v)*((T4+v*T5)+(v*v)*T6)));
		    return (hx>=0)? NONE/z: ONE/z;
		}
	    }
	    if(((i-392)^(i-412))<0) {		/* if(i>=392&&i<412) */
	    		/* near 2pi, y=x-2pi,sin(x)=sin(y),cos(x)=cos(y) */
		p = x-PI2_H;		
		i = ix-0x401921fb;
		x = p-PI2_L;
		if((i|((lx>>8)-0x54442D))==0) {/* very close to 2pi */
		    x = p-PI2_L0;
		    return (hx>=0)? (x-PI2_L1):(PI2_L1-x);
		} else {
		    v = x*x;
		    z = p-(PI2_L-(x*v)*(((T1+T2*v)+(v*v)*T3)+
				((v*v)*v)*((T4+v*T5)+(v*v)*T6)));
		    return (hx>=0)? z: -z;
		}
	    }
	    if(((i-493)^(i-513))<0) {		/* if(i>=493&&i<513) */
	    		/* near 5pi/2, y=x-5pi/2, tan(x)=-1/tan(y) */
		p = x-PI5O2_H;
		i = ix-0x401F6A7A;
		x = p-PI5O2_L;
		if((i|((lx>>8)-0x295538))==0) {/* very close to 5pi/2 */
		    x = p-PI5O2_L0;
		    return (hx<0)? ONE/(x-PI5O2_L1): ONE/(PI5O2_L1-x);
		} else {
		    v = x*x;
		    z = p-(PI5O2_L-(x*v)*(((T1+T2*v)+(v*v)*T3)+
				((v*v)*v)*((T4+v*T5)+(v*v)*T6)));
		    return (hx>=0)? NONE/z: ONE/z;
		}
	    }
	    p = v+(v*s)*(PP1+s*PP2);
	    q = s*(QQ1+s*QQ2);
	    w = _TBL_sin[j];
	    z = _TBL_cos[j];
	    t = z-(w*p-z*q);
	    s = (w*q+z*p);
	    return (hx>=0)? (w+s)/t:-(w+s)/t;
	}

    /* tan(Inf or NaN) is NaN */
	else if (ix>=0x7ff00000) {
                if (ix == 0x7ff00000 && lx == 0)
	            __set_errno (EDOM);
	        return x-x;
	}

    /* argument reduction needed */
	else {
                y[0] = __rem_pio2 (x, &n, &y[1]);

	        p = __k_sincos_(y,&q);
	        return (n & 1) == 0 ? p / q : -q / p;
	}
}

weak_alias (__tan, tan)
