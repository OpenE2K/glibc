#include <bits/wordsize.h>

#if __WORDSIZE == 32
# define MOVPL		getpl
# define LDD		ldgdd
# define LDP		ldgdw
# define CMPEP		cmpesb
# define UC_LINK_OFFSET	4
#else /* __WORDSIZE == 64  */
# define MOVPL		movtd
# define LDD		ldd
# define LDP		ldw
# define CMPEP		cmpedb
# define UC_LINK_OFFSET	8
#endif /* __WORDSIZE == 64  */

	.global makecontext_helper
	.type makecontext_helper, @function
makecontext_helper:
	{
	  setwd wsz = 0x8, nfx = 0x1
	  setbn rsz = 0x3, rbs = 0x4, rcur = 0x0
	}

	MOVPL %r1, %ctpr1

	/* Get the first 4 register arguments (not counting the one on %qb[0])
	   to the target function from our incoming registers and ...  */
	movtq %r4, %b[0]
	movtq %r6, %b[2]

	/* Reuse %r1 for %fp now that it has been used to set %ctpr1.  */
	getsp 0x0, %r1

	/* ... the trailing 4 register arguments from our stack frame.  */
	cmpbesb %r2, 4, %pred0
	LDD %r1, 0x40, %b[4] ? ~%pred0

	cmpbesb %r2, 5, %pred0
	LDD %r1, 0x48, %b[5] ? ~%pred0

	cmpbesb %r2, 6, %pred0
	LDD %r1, 0x50, %b[6] ? ~%pred0

	cmpbesb %r2, 7, %pred0
	LDD %r1, 0x58, %b[7] ? ~%pred0

	/* "free ()" space on stack allocated for our 4 subsidiary
	   parameters so as the target function does not "see" them.  */
	getsp 0x20, %empty
	call %ctpr1, wbs = 0x4

	return %ctpr3
	/* See whether `ctx->uc_link == NULL' and call exit () if so. Otherwise
	   just return.  */
	LDP %r0, UC_LINK_OFFSET, %r0
	CMPEP %r0, 0x0, %pred0
	ct %ctpr3 ? ~%pred0


#ifdef SHARED
	disp %ctpr1, __GI_exit
#else /* ! defined SHARED  */
	disp %ctpr1, exit
#endif /* ! defined SHARED  */

	adds 0x0, 0x0, %b[0]
	call %ctpr1, wbs = 0x4

	.size makecontext_helper, . - makecontext_helper
