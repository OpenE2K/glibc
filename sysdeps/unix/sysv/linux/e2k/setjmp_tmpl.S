/* Copyright (C) 1997, 1999 Free Software Foundation, Inc.
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

/* FIXME */
#undef r0
#undef r1

#ifndef _jmp_buf_defined
#define _jmp_buf_defined

!
! `struct jmp_buf' is defined in `sysdeps/unix/sysv/linux/e2k/bits/setjmp.h'
!
! struct jmp_buf
! {
!   uint64 sigmask;    /* The uppermost bit indicates if (SIGKILL | SIGSTOP) should be restored.  */
!   uint64 ip;         /* %cr0.hi  */
!   uint64 cr1lo;      /* %cr1.lo (contains wbs and wpsz) */
!   uint64 pcsplo;     /* %pcsp.lo  */
!   uint64 pcsphi;     /* %pcsp.hi  */
!   uint32 pcshtp;     /* %pcshtp  */
!   uint32 br;         /* (the lower 28 bits of) %br  */
!   uint64 usdlo;      /* %usd.lo (used by GLIBC)  */
!   uint32 reserv1;
!   uint32 psz;        /* (the lower 11 bits of) %wd.psz  */
!   uint64 reserv2;
!   uint64 reserv3;
! };
!
! Offsets in bytes of `struct jmp_buf' fields:
!

#define jmpbuf_sigmask   0x00
#define jmpbuf_ip        0x08
#define jmpbuf_cr1lo     0x10
#define jmpbuf_pcsplo    0x18
#define jmpbuf_pcsphi    0x20
#define jmpbuf_pcshtp    0x28
#define jmpbuf_br        0x2C
#define jmpbuf_usdlo     0x30
#define jmpbuf_psz       0x38
#define jmpbuf_reserv1   0x38
#define jmpbuf_reserv2   0x40
#define jmpbuf_reserv3   0x48
#define jmpbuf_size      0x50

!
! Load/store instructions and parameter passing conventions vary between
! different compilation modes.
!

#define LDD	__LDD
#define STD	__STD
#define STW	__STW

#if ! defined __PROTECTED__

#define ARG0	%r0
#define ARG1	%r1
#define TEMP0	%r2
#define TEMP1	%r3
#define TEMP2	%r4
#define TEMP3	%r5
#define RES     %r0

#else /* defined __PROTECTED__  */


#define ARG0	    %r2
#define ARG1	    %r4
#define RES	    %r0
#define TEMP0	    %r6
#define TEMP1	    %r7
#define TEMP2	    %r8
#define TEMP3	    %r9

#endif /* defined __PROTECTED__  */

#endif /* _jmp_buf_defined */

!
!  int setjmp(jmp_buf buffer)
!

	.text
	.align	8
	.global	FUNCNAME
	.type	FUNCNAME,@function
        .ignore ld_st_style
#if defined __ptr128__
	.hidden FUNCNAME
#endif
FUNCNAME:
1:
	{
#if ! defined __ptr128__
	    setwd   wsz = 0x5
#else /* defined __ptr128__  */
	    setwd   wsz = 0x8
#endif /* defined __ptr128__  */
	    rrd     %cr0.hi,  TEMP1
#ifdef HAS_SECOND_ARG
	    cmpesb  ARG1, 0,  %pred0
#endif
	    addd	0, 0, 	  TEMP0
#if !(defined(NEED_MASK) && !defined(HAS_SECOND_ARG))
	    return	%ctpr3
#endif
	}
	{
	    STD     ARG0, jmpbuf_reserv2, TEMP0
	    STD     ARG0, jmpbuf_ip, TEMP1
	}	
	{
	    STD     ARG0, jmpbuf_reserv3, TEMP0
	    STD     ARG0, jmpbuf_sigmask, TEMP0
	}	
!
! %pcsp.{lo,hi} and %pcshtp registers should be read atomically here. Because
! rrd may be encoded only in ALC0, make use of the following trick: load
! `jmpbuf.pcsphi' with "lock wait" and modify it after having read all the
! three registers in "wait & unlock" mode. Since the lock is disabled by an
! interrupt, the one which may have changed the registers will trigger RBRANCH.
!
! ATTENTION: there should be no memory accesses between lock and unlock (see
! Bug #33705)
!
! ATTENTION: to make rrd execution take a single tick, ALC2 should be used to
! store its result.
!
	{
#ifdef NEED_MASK
	    disp    %ctpr1,   $__sigjmp_save
#endif
	    LDD,0   ARG0, jmpbuf_pcsphi, TEMP1, mas=0x7
	}
	{
	    rrd     %pcsp.lo, TEMP1
	}
	{
	    rrd     %pcsp.hi, TEMP2
	}
	{
	    rrs     %pcshtp,  TEMP3
	}
	{
	    STD,2   ARG0, jmpbuf_pcsphi, TEMP2, mas=0x2
	    rbranch 1b
	}
	{
	    rrd     %wd,      TEMP1
	    STD     ARG0, jmpbuf_pcsplo, TEMP1
	    STW     ARG0, jmpbuf_pcshtp, TEMP3
	}
	{
	    rrd     %cr1.lo,  TEMP1
	    STD     ARG0, jmpbuf_psz, TEMP1
	}
	{
	    rrs     %cr1.hi,  TEMP1
	    STD     ARG0, jmpbuf_cr1lo, TEMP1
	    STW     ARG0, jmpbuf_reserv1, TEMP0
	}
	{
	    rrd     %usd.lo,  TEMP1
	    STW     ARG0, jmpbuf_br, TEMP1
	}
	{
	    STD,2   ARG0, jmpbuf_usdlo, TEMP1
#ifndef NEED_MASK
	    addd    0, 0,     RES
	    ct      %ctpr3
	}
#else
!
! ATTENTION: this is a tail call, its parameter is already in ARG0.
!
#  ifndef HAS_SECOND_ARG
	    ct      %ctpr1
	}
#  else
	    ct      %ctpr1 ? ~%pred0
	}
	{
	    addd    0, 0,     RES
	    ct      %ctpr3
	}
#  endif
#endif
	.size	FUNCNAME, .-FUNCNAME
