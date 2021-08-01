/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* FPU control word bits.  i387 version.
   This file is part of the GNU C Library.
   Contributed by Olaf Flebbe.

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

#ifndef _FPU_CONTROL_H
#define _FPU_CONTROL_H	1

/* Here is the dirty part. Set up your E2k through the %pfpfr register.
 *
 *  15 14-13  12   11   10   9    8    7   6-0
 * |..| RC  | PM | UM | OM | ZM | DM | IM |...|
 *
 * IM: Invalid operation mask
 * DM: Denormalized operand mask
 * ZM: Zero-divide mask
 * OM: Overflow mask
 * UM: Underflow mask
 * PM: Precision (inexact result) mask
 *
 * Mask bit is 1 means no interrupt.
 *
 * PC: Precision control
 * only in %fpcr register
 *
 * RC: Rounding control
 * 00 - rounding to nearest
 * 01 - rounding down (toward - infinity)
 * 10 - rounding up (toward + infinity)
 * 11 - rounding toward zero
 *
 * The hardware default is 0x1fbf which we use.
 */

#include <features.h>

/* masking of interrupts in %pfpfr */
#define _FPU_MASK_IM  0x0080
#define _FPU_MASK_DM  0x0100
#define _FPU_MASK_ZM  0x0200
#define _FPU_MASK_OM  0x0400
#define _FPU_MASK_UM  0x0800
#define _FPU_MASK_PM  0x1000

/* masking of interrupts in %fpcr */
#define _FPU_FPCR_MASK_IM  0x0001
#define _FPU_FPCR_MASK_DM  0x0002
#define _FPU_FPCR_MASK_ZM  0x0004
#define _FPU_FPCR_MASK_OM  0x0008
#define _FPU_FPCR_MASK_UM  0x0010
#define _FPU_FPCR_MASK_PM  0x0020

/* precision control (for %fpcr only) */
#define _FPU_FPCR_EXTENDED 0x0300  /* RECOMMENDED bits in %fpcr */
#define _FPU_FPCR_DOUBLE   0x0200
#define _FPU_FPCR_SINGLE   0x0000

/* rounding control in %pfpfr */
#define _FPU_RC_NEAREST 0x00000    /* RECOMMENDED bits in %pfpfr */
#define _FPU_RC_DOWN    0x02000
#define _FPU_RC_UP      0x04000
#define _FPU_RC_ZERO    0x06000

/* rounding control in %fpcr */
#define _FPU_RC_FPCR_NEAREST 0x000    /* RECOMMENDED bits in %fpcr */
#define _FPU_RC_FPCR_DOWN    0x400
#define _FPU_RC_FPCR_UP      0x800
#define _FPU_RC_FPCR_ZERO    0xC00

#define _FPU_RESERVED 0xFFFF807F  /* Reserved bits in %pfpfr */
#define _FPU_FPCR_RESERVED 0x0FFFF0C0 /* Reserved bits in %fpcr */


/* The fdlibm code requires strict IEEE double precision arithmetic,
   and no interrupts for exceptions, rounding to nearest.  */

#define _FPU_DEFAULT  0x1fbf
#define _FPU_FPCR_DEFAULT 0x37f

/* IEEE:  same as above.  */
#define _FPU_IEEE     0x1fbf

/* Type of the control word.  */
typedef unsigned int fpu_control_t __attribute__ ((__mode__ (__HI__)));

/* Macros for accessing the hardware control word.  */
#define _FPU_GETCW(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rrs %%fpcr, %0" : "=r" (cw))
#define _FPU_SETCW(cw) _Pragma ("asm_inline") __asm__ __volatile__ ("rws %0, %%fpcr" : : "r" (cw))
#define _FPU_GETPFPFR(pfpfr) _Pragma ("asm_inline") __asm__ __volatile__ ("rrs %%pfpfr, %0" : "=r" (pfpfr))
#define _FPU_SETPFPFR(pfpfr) _Pragma ("asm_inline") __asm__ __volatile__ ("rws %0, %%pfpfr" : : "r" (pfpfr))

/* Default control word set at startup.  */
extern fpu_control_t __fpu_control;

#endif	/* fpu_control.h */
