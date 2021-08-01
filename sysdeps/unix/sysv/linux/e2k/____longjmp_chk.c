#include <setjmp.h>
#include <stdio.h>

extern void ____longjmp_chk (__jmp_buf __env, int __val)
  __attribute__ ((__noreturn__));

void
____longjmp_chk (__jmp_buf env, int val)
{
  /* On E2K `longjmp ()' is implemented by means of a system call. Therefore,
     the Kernel should be capable of performing all necessary checks and return
     in case of an invalid ENV. FIXME: it's likely to kill the process under
     some circumstances instead, which is to be revised.  */
    __longjmp (env, val);

  __fortify_fail ("longjmp causes uninitialized stack frame");
}
