#include <asm/errno.h>
#include <asm/unistd.h>
#include <sysdep.h>

#define CLONE_VM		0x00000100
#define CLONE_THREAD		0x00010000
#define CLONE_SETTLS		0x00080000
#define CLONE_PARENT_SETTID	0x00100000
#define CLONE_CHILD_CLEARTID	0x00200000
#define CLONE_CHILD_SETTID	0x01000000

#undef r0
#undef r1

#define SYS_NUM_REG		%b[0]

#define FIRST_PARAM_REG		%qb[2]
#define SECOND_PARAM_REG	%qb[4]
#define THIRD_PARAM_REG		%qb[6]
#define FOURTH_PARAM_REG	%qb[8]
#define FIFTH_PARAM_REG		%qb[10]

#define SYSCALL_RES	%b[0]

#define WSZ		0x10
#define RSZ		0x7
#define RBS		0x8
#define WBS		0x8

/* int __clone (int (*fn)(void *arg), void *child_stack, int flags, void *arg,
                pid_t *ptid, struct user_desc *tls, pid_t *ctid) */

	.ignore ld_st_style
	.text
	.align 8
	.global	__clone
	.type	__clone,@function

__clone:

	setwd	wsz = WSZ
	setbn	rsz = RSZ, rbs = RBS, rcur = 0x0
	/* Save the thread function's argument (it has been passed to us on
	   stack) into a register.  */
        ldapq,0 %qr0, 0x40, %qr8
	
	/* Sanity check arguments */
	disp	%ctpr1, _error
	/* Verify the function's pointer.  */
        puttagd,2 %dr2, 0, %dr10
        cmpedb,0 %dr10, 0, %pred0

	/* Verify the child_stack.  */
        getva,0 %qr4, %r5, %dr10
        cmpedb,0 %dr10, 0, %pred1

	ct	%ctpr1 ? %pred0
	ct	%ctpr1 ? %pred1

	/* Get ready to perform the syscall. */
	adds	0x0, __NR_clone, SYS_NUM_REG

	/* Flags are passed as the first argument.  */
	sxt 0x6, %r6, FIRST_PARAM_REG

	/* The stack pointer is passed as the second argument.  */
	movtq   %qr4, SECOND_PARAM_REG

	disp	%ctpr1, no_tls

	/* Below I reuse %qr6 the original value of which has already been
	   copied into %db[1].  */
	ands	%r6, (CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID), %r6
	cmpesb	%r6, 0x0, %pred0
	ct	%ctpr1 ? %pred0

	/* The remaining three arguments are variadic and turn out to be
	   passed at the same offsets with respect to %fp (%qr0) whichever
	   ABI is used.   */

	/* Copy ptid.  */
	ldapq	%qr0, 0x50, %qr6
	movtq %qr6, THIRD_PARAM_REG

	/* Copy tls.  */
	ldapq	%qr0, 0x60, %qr6
	movtq %qr6, FIFTH_PARAM_REG

	/* Copy ctid.  */
	ldapq	%qr0, 0x70, %qr6
	movtq %qr6, FOURTH_PARAM_REG

no_tls:	
	sdisp	%ctpr1, __SYSCALL_TRAPNUM
	call	%ctpr1, wbs = WBS

	/* Check the result */
	disp	%ctpr1, _error
	subs	0x0, SYSCALL_RES, %r7
	disp	%ctpr2, __thread_start

        /* FIXME: one should probably ensure that %b[0] belongs to
	   (-125, -1) range, not that it's just less than zero. */
	cmplsb	SYSCALL_RES, 0x0, %pred0
	cmpesb	SYSCALL_RES, 0x0, %pred1
	ct	%ctpr1 ? %pred0
	ct	%ctpr2 ? %pred1

	/* Return new thread's pid */
	adds	0x0, SYSCALL_RES, %r0

	return	%ctpr3
	ct	%ctpr3

_error:
	adds	0x0, EINVAL, %r7
	disp	%ctpr1, __errno_location
	call	%ctpr1, wbs = WBS
	stapw	%qb[0], 0, %r7
	adds	0x0, -1, %r0

	return	%ctpr3
	ct	%ctpr3


	.size __clone, .-__clone

	.type __thread_start,@function

__thread_start:

	/* Use the thread function's argument saved in `%qr8' above.  */
        movtq,0 %qr8, %qb[2]

        getsap  -0x20, %qr8
        movtq,0 %qr8, %qb[0]

	/* Get ready to call the thread function via its pointer in `%qr2'. */
        movtd,0 %dr2, %ctpr1
	call	%ctpr1, wbs = WBS

	/* Pass the thread function's return value (it's already on `%b[0]')
	   to exit syscall.  */
        adds 0, %b[0], FIRST_PARAM_REG
	adds	0x0, __NR_exit, SYS_NUM_REG
	sdisp	%ctpr1, __SYSCALL_TRAPNUM
	call	%ctpr1, wbs = WBS

	.size __thread_start, .-__thread_start

libc_hidden_def (__clone)
weak_alias (__clone, clone)
