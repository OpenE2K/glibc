/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Copyright (C) 1997, 1998, 1999, 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#ifndef _FENV_H
# error "Never use <bits/fenv.h> directly; include <fenv.h> instead."
#endif


/* Define bits representing the exception.
   We use ie, ze, oe, ue, pe bits of PFPFR/FPSR registers.  */
enum
  {
    FE_INVALID =
#define FE_INVALID	0x01
      FE_INVALID,
    __FE_DENORM = 0x02,
    FE_DIVBYZERO =
#define FE_DIVBYZERO	0x04
      FE_DIVBYZERO,
    FE_OVERFLOW =
#define FE_OVERFLOW	0x08
      FE_OVERFLOW,
    FE_UNDERFLOW =
#define FE_UNDERFLOW	0x10
      FE_UNDERFLOW,
    FE_INEXACT =
#define FE_INEXACT	0x20
      FE_INEXACT
  };

#define FE_ALL_EXCEPT \
	(FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

/* The E2K supports all of the four defined rounding modes.  We
   use again the bit positions in the PFPFR register as the values
   for the appropriate macros.  */
enum
  {
    FE_TONEAREST =
#define FE_TONEAREST	0x0000
      FE_TONEAREST,
    FE_DOWNWARD =
#define FE_DOWNWARD	0x2000
      FE_DOWNWARD,
    FE_UPWARD =
#define FE_UPWARD	0x4000
      FE_UPWARD,
    FE_TOWARDZERO =
#define FE_TOWARDZERO	0x6000
      FE_TOWARDZERO
  };

#define __FE_ROUND_MASK	0x6000

/* The bit positions in the FPCR register.  */
enum
  {
    FE_FPCR_TONEAREST =
#define FE_FPCR_TONEAREST	0
      FE_FPCR_TONEAREST,
    FE_FPCR_DOWNWARD =
#define FE_FPCR_DOWNWARD	0x400
      FE_FPCR_DOWNWARD,
    FE_FPCR_UPWARD =
#define FE_FPCR_UPWARD		0x800
      FE_FPCR_UPWARD,
    FE_FPCR_TOWARDZERO =
#define FE_FPCR_TOWARDZERO	0xc00
      FE_FPCR_TOWARDZERO
  };

#define __FE_FPCR_ROUND_MASK	0xc00


/* Type representing exception flags.  */
typedef unsigned int fexcept_t;

/* Type representing floating-point environment.  */
typedef struct
  {
    unsigned int __fpcr;
    unsigned int __fpsr;
    unsigned int __pfpfr;
  }
fenv_t;

/* If the default argument is used we use this value.  */
#define FE_DFL_ENV	((__const fenv_t *) -1)

#ifdef __USE_GNU
/* Floating-point environment where none of the exception is masked.  */
# define FE_NOMASK_ENV	((__const fenv_t *) -2)
#endif

/* For internal use only: access the fp state register.  */
#define __fenv_getpfpfr(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rrs %%pfpfr, %0" : "=r" (cw))
#define __fenv_setpfpfr(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rws %0, %%pfpfr" : : "r" (cw))

#define __fenv_getfpcr(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rrs %%fpcr, %0" : "=r" (cw))
#define __fenv_setfpcr(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rws %0, %%fpcr" : : "r" (cw))

#define __fenv_getfpsr(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rrs %%fpsr, %0" : "=r" (cw))
#define __fenv_setfpsr(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rws %0, %%fpsr" : : "r" (cw))

#if __GLIBC_USE (IEC_60559_BFP_EXT)
/* Type representing floating-point control modes.  */
typedef struct
  {
    unsigned int __fpcr;
    unsigned int __pfpfr;
  }
femode_t;

/* Default floating-point control modes.  */
# define FE_DFL_MODE	((const femode_t *) -1L)
#endif
