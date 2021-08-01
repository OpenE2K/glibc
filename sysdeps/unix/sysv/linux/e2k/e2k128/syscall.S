#include <sysdep.h>

        .ignore ld_st_style
	.text

ENTRY (syscall)

{
	! wsz = 0x9 is used here for consistency with macros in <sysdep.h>,
	! otherwise `wsz = 0x8' would be quite OK. `setbn' is used here for the
	! same reason.
	setwd   wsz = WSZ
	setbn   rsz = WSZ - WBS - 1, rbs = WBS, rcur = 0x0
}
	disp %ctpr1, do_syscall

	puttagd %dr1, 0x0, %dr2
        shrd %dr2, 32, %dr2

	cmpbsb %r2, 32, %pred0
	ct %ctpr1 ? %pred0
	/* Load syscall number.  */
	ldapq %qr0, 16, %qb[0]

	cmpbsb %r2, 48, %pred0
	ct %ctpr1 ? %pred0
	/* Load syscall argument #1.  */
	ldapq %qr0, 32, %qb[2]

	cmpbsb %r2, 64, %pred0
	ct %ctpr1 ? %pred0
	/* Load syscall argument #2.  */
	ldapq %qr0, 48, %qb[4]

	cmpbsb %r2, 80, %pred0
	ct %ctpr1 ? %pred0
	/* Load syscall argument #3.  */
	ldapq %qr0, 64, %qb[6]
	
#if defined __ptr128_new_abi__
	/* Load syscall argument #4.  */
	cmpbsb %r2, 96, %pred0
	ct %ctpr1 ? %pred0
	ldapq %qr0, 80, %qb[8]

	cmpbsb %r2, 112, %pred0
	ct %ctpr1 ? %pred0
	/* Load syscall argument #5.  */
	ldapq %qr0, 96, %qb[10]

	cmpbsb %r2, 128, %pred0
	ct %ctpr1 ? %pred0
	/* Load syscall argument #6.  */
	ldapq %qr0, 112, %qb[12]
#endif /* defined __ptr128_new_abi__  */

do_syscall:
        sdisp   %ctpr1, __SYSCALL_TRAPNUM
        call    %ctpr1, wbs = WBS

        __SYSCALL_OUTPUT

	ret

PSEUDO_END (syscall)
