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

	/* Taking into account that we can be called with the same context 
	   more than once with no repetetive preparations, we may NOT clobber
	   any of %r{0,1,2} which should keep their original values between
	   invocations. Moreover, it would be better not to spoil %r{4,5,6,7}
	   containing the first 4 arguments for the target function either so
	   as to ensure that it gets the same values in them across multiple
	   invocations. Note, however, that this cannot be guaranteed with
	   respect to subsequent target function's arguments arriving (here)
	   exclusively on stack as it's capable of modifying them when spilling
	   to stack.  */
	getsp 0x0, %b[7]

	/* ... the trailing 4 register arguments from our stack frame.  */
	cmpbesb %r2, 4, %pred0
	LDD %b[7], 0x40, %b[4] ? ~%pred0

	cmpbesb %r2, 5, %pred0
	LDD %b[7], 0x48, %b[5] ? ~%pred0

	cmpbesb %r2, 6, %pred0
	LDD %b[7], 0x50, %b[6] ? ~%pred0

	cmpbesb %r2, 7, %pred0
	LDD %b[7], 0x58, %b[7] ? ~%pred0
	/* Zero out %sp on %b[7] if it hasn't been rewritten by an incoming
	   argument one line above.  */
	addd 0x0, 0x0, %b[7] ? %pred0

	/* "free ()" space on stack allocated for our 4 subsidiary
	   parameters so as the target function does not "see" them.  */
	getsp 0x20, %empty
	call %ctpr1, wbs = 0x4

	return %ctpr3
	/* See whether `ctx->uc_link == NULL' and call exit () if so. Otherwise
	   just return.  */
	LDP %r0, UC_LINK_OFFSET, %b[0]
	CMPEP %b[0], 0x0, %pred0
	ct %ctpr3 ? ~%pred0


#ifdef SHARED
	disp %ctpr1, __GI_exit
#else /* ! defined SHARED  */
	disp %ctpr1, exit
#endif /* ! defined SHARED  */

	/* FIXME(?): isn't GETSP needed here to be on the safe side?  */

	adds 0x0, 0x0, %b[0]
	call %ctpr1, wbs = 0x4

	.size makecontext_helper, . - makecontext_helper
