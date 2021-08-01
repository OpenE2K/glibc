#ifndef PREINIT_FUNCTION
# define PREINIT_FUNCTION $__gmon_start__
#endif

#ifndef PREINIT_FUNCTION_WEAK
# define PREINIT_FUNCTION_WEAK 1
#endif

#if PREINIT_FUNCTION_WEAK
	weak_extern (PREINIT_FUNCTION)
#else
	.hidden PREINIT_FUNCTION
#endif

    .section ".init", "ax", @progbits
    .align 8
    .global $_init
    .type $_init,@function
$_init:
    {
      setwd wsz = 0xa, nfx = 0x1
      nop 1
      rrd %ip, %r0
      addd 0x0, [$_GLOBAL_OFFSET_TABLE_], %r1
    }
    addd,2 %r0, %r1, %r0
    ldgdw  %r0, [PREINIT_FUNCTION@GOT], %r0
#if PREINIT_FUNCTION_WEAK
    /* Does `nop 4' below have any relation to comparison? Is it required in
       unconditional case, i.e. when `PREINIT_FUNCTION_WEAK == 0'.  */
    {
      nop 4
      getpl %r0, %ctpr1
      cmpedb %r0, 0x0, %pred0
    }
    call %ctpr1, wbs = 0x6 ? ~%pred0
#else /* PREINIT_FUNCTION_WEAK  */
    {
      nop 4
      getpl %r0, %ctpr1
    }
    call %ctpr1, wbs = 0x6
#endif /* PREINIT_FUNCTION_WEAK  */

    .section ".fini", "ax", @progbits
    .align 8
    .global $_fini
    .type $_fini,@function
$_fini:
    setwd wsz = 0xa, nfx = 0x1
/* Currently there is a bug in GAS: it has been noted empirically that it
   doesn't encode the "braceless" instruction which occupies the last line
   in a file. Remove this comment when it is fixed.  */
