/* Copyright (C) 2000-2013 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* I wonder, why Sparc's version isn't protected from double inclusion.  */
#ifndef _BITS_SIGCONTEXT_H
#define _BITS_SIGCONTEXT_H  1

#if !defined _SIGNAL_H && !defined _SYS_UCONTEXT_H
# error "Never use <bits/sigcontext.h> directly; include <signal.h> instead."
#endif


/* These constants are used to define `struct sigcontext' only, they are not
   to be exported to user. Therefore, we undefine them below.  */
#define MLT_NUM                 48
#define MAX_TC_SIZE             10
#define TIR_NUM                 19
#define DAM_ENTRIES_NUM         32
#define SBBP_ENTRIES_NUM        32


/* All definitions below are borrowed from
   `arch/e2k/include/asm/sigcontext.h'.  */

struct sigcontext {
	unsigned long long	cr0_lo;
	unsigned long long	cr0_hi;
	unsigned long long	cr1_lo;
	unsigned long long	cr1_hi;
	unsigned long long	sbr;	 /* 21 Stack base register: top of */
				 	 /*    local data (user) stack */
	unsigned long long 	usd_lo;	 /* 22 Local data (user) stack */
	unsigned long long 	usd_hi;	 /* 23 descriptor: base & size */
	unsigned long long	psp_lo;	 /* 24 Procedure stack pointer: */
	unsigned long long 	psp_hi;	 /* 25 base & index & size */
	unsigned long long 	pcsp_lo; /* 26 Procedure chain stack */
	unsigned long long 	pcsp_hi; /* 27 pointer: base & index & size */

  /* The kernel believes that the following fields are missing from PM
     variant of this struct.  */
#if ! defined __ptr128__
/*
 *  additional part (for binary compiler)
 */          
        unsigned long long rpr_hi;
        unsigned long long rpr_lo;
        
	unsigned long long nr_TIRs;
        unsigned long long tir_lo [TIR_NUM];
	unsigned long long tir_hi [TIR_NUM];

	unsigned long long trap_cell_addr [MAX_TC_SIZE];
	unsigned long long trap_cell_val  [MAX_TC_SIZE];
	unsigned char      trap_cell_tag  [MAX_TC_SIZE];
	unsigned long long trap_cell_info [MAX_TC_SIZE];

	unsigned long long dam [DAM_ENTRIES_NUM];

	unsigned long long sbbp [SBBP_ENTRIES_NUM];
        
	unsigned long long mlt [MLT_NUM];

        unsigned long long upsr;
#endif /* ! defined __ptr128__  */
};

#undef MLT_NUM
#undef MAX_TC_SIZE
#undef TIR_NUM
#undef DAM_ENTRIES_NUM
#undef SBBP_ENTRIES_NUM


struct extra_ucontext {
  int sizeof_extra_uc;
  int		   curr_cnt;/* current index into trap_celler */
  int		   tc_count;/* numbers of trap_celler's elements */

  /* For getcontext ().  */
  int fpcr;
  int fpsr;
  int pfpfr;

  unsigned long long ctpr1;
  unsigned long long ctpr2;
  unsigned long long ctpr3;
};

#endif /* _BITS_SIGCONTEXT_H  */
