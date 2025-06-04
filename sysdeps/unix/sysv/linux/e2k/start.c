/* This file is part of the GNU C Library.

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

#if defined __ptr128__
extern void __selfinit (void) __attribute__ ((visibility ("hidden")));
#endif

extern int main (int argc, char **argv)
#if defined IN_RSTART
/* Let &main in _start () below be evaluated as `_GLOBAL_OFFSET_TABLE_ +
   link-time offset' rather than by loading it from GOT. This is crucial when
   compiling this code for rcrt1.o linked into -static-pie executables as it's
   executed BEFORE any dynamic relocations including the ones in .rela.got are
   processed which may result in {link != run}-time value of &main passed to
   `__libc_start_main ()'.

   TODO: find out if static-start.c can be employed (by analogy with how it is
   in PM) to distinguish this case via a macro.  */
  __attribute__ ((visibility ("hidden")))
#endif /* defined IN_RSTART  */
  ;

extern int
__libc_start_main (int (*main) (int, char **, char **),
                   int argc,
                   char **argv_and_env,
                   void (*init) (int, char **, char **),
                   void (*fini) (void),
                   void (*rtld_fini) (void),
                   void *stack_end);

void
#if ! defined __LCC__ && ! defined SHARED
__attribute__ ((optimize ("-fno-function-cse")))
#endif /* ! defined __LCC__ && ! defined SHARED  */
_start (register void (*rtld_fini)(void))
{
  /* The parameters to main () are passed on stack starting with `argc',
     `rtld_fini' is passed on %dr0. Standard calling conventions require
     space on stack to be reserved for `rtld_fini', however, the Kernel is
     likely to put `argc' at that location. Besides that the Kernel doesn't
     probably take care of 8-byte alignment for (which?) parameters. That's
     why some hackery is required to account for all that.  */
#if ! defined __ptr128__
  register long *fp asm ("FP");
#else /* defined __ptr128__  */
  /* In Protected Mode it would be incorrect to obtain an incoming stack frame
     via `getsap 0x0' since it would be useless because of its zero size. Make
     use of AP passed by the kernel on %qr0 instead.  */
  register void **fp asm ("%r0");
  /* Space should be reserved by the Kernel at the bottom of the stack frame
     addressed by `%qr0' for  `%qr0' itself.  */
  fp += 1;

  /* __selfinit () is not currently to be used only in glibc that is linked
     with GOLD. Eventually its use will hopefully be removed from
     e2k-linux-ld.bfd too and this compilation time test will have to be
     replaced with a runtime one (for the presence of a bit in ELF header
     or something like that).  */
# if ! defined HAVE_E2K_GOLD
  /* In dynamic case ld.so has already called `__selfinit ()' for us (i.e. the
     main executable). Calling it for the second time may result in SIGSEGV
     because of the GNU_RELRO segment which has already been write protected
     by ld.so.  */
  if (rtld_fini == (void (*) (void)) 0)
    __selfinit ();
# endif /* ! defined HAVE_E2K_GOLD  */
#endif /* defined __ptr128__  */

  int argc = *((int *) fp);
  char **argv = (char**)(fp + 1);

  /* envp = &argv[argc+1] */
  __libc_start_main ((int (*) (int, char **, char**)) &main, argc, argv,
		     (void (*) (int, char **, char **)) 0 /* __libc_csu_init  */,
		     (void (*) (void)) 0 /* __libc_csu_fini  */,
                     rtld_fini,
                     0);

  /* Die very horribly if exit returns.  */
  { int *p = 0; *p = 0; }
}
