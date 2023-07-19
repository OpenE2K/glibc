	.global makecontext_helper
	.type makecontext_helper, @function
makecontext_helper:
	{
	  setwd wsz = 0x10, nfx = 0x1
	  setbn rsz = 0x7, rbs = 0x8, rcur = 0x0
	}

	movtd %r4, %ctpr1

	aptoap %r0, 0x30, %b[0]

	/* Get the first 4 register arguments (not counting the one on %qb[0])
	   to the target function from our incoming registers and ...  */
	movtq %r8, %b[2]
	movtq %r10, %b[4]
	movtq %r12, %b[6]
	movtq %r14, %b[8]

	/* ... the trailing 3 register arguments from our stack frame.  */
	cmpbesb %r6, 4, %pred0
	ldapq %r0, 0x80, %b[10] ? ~%pred0

	cmpbesb %r6, 5, %pred0
	ldapq %r0, 0x90, %b[12] ? ~%pred0

	cmpbesb %r6, 6, %pred0
	ldapq %r0, 0xa0, %b[14] ? ~%pred0


	call %ctpr1, wbs = 0x8

	return %ctpr3
	/* See whether `ctx->uc_link == NULL' and call exit () if so. Otherwise
	   just return.  */
	ldapq %r2, 0x10, %r2

	/* Compare `void (*) (void)' in %qr2 against NULL by analogy
	   with LCC.  */
	puttagd %r2, 0x0, %r2
	andd %r2, 0xffffffffffff, %r2
	cmpedb %r2, 0x0, %pred0
	ct %ctpr3 ? ~%pred0


#ifdef SHARED
	disp %ctpr1, __GI_exit
#else /* ! defined SHARED  */
	disp %ctpr1, exit
#endif /* ! defined SHARED  */

	getsap -32, %b[0]
	adds 0x0, 0x0, %b[2]
	call %ctpr1, wbs = 0x8

	.size makecontext_helper, . - makecontext_helper
