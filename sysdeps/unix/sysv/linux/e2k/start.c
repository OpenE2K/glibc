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

extern void __libc_csu_init (int argc, char **argv, char **envp);
extern void __libc_csu_fini (void);

extern int main (int argc, char **argv);

extern int
__libc_start_main (int (*main) (int, char **, char **),
                   int argc,
                   char **argv_and_env,
                   void (*init) (int, char **, char **),
                   void (*fini) (void),
                   void (*rtld_fini) (void),
                   void *stack_end);

void 
_start (
#if ! defined IN_STATIC_START
        register void (*rtld_fini)(void)
#else /* ! defined IN_STATIC_START  */
	void
#endif /* ! defined IN_STATIC_START  */
        )
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

  /* In dynamic case ld.so has already called `__selfinit ()' for us (i.e. the
     main executable). Calling it for the second time may result in SIGSEGV
     because of the GNU_RELRO segment which has already been write protected
     by ld.so.  */
# if ! defined IN_STATIC_START
  if (rtld_fini == (void (*) (void)) 0)
# endif /* ! defined IN_STATIC_START  */
    __selfinit ();
#endif /* defined __ptr128__  */

  int argc = *((int *) fp);
  char **argv = (char**)(fp + 1);

  /* envp = &argv[argc+1] */
  __libc_start_main ((int (*) (int, char **, char**)) &main, argc, argv,
		     __libc_csu_init, __libc_csu_fini,
#if ! defined IN_STATIC_START
                     rtld_fini,
#else /* defined IN_STATIC_START  */
                     (void (*) (void)) 0,
#endif /* defined IN_STATIC_START  */
                     0);

  /* Die very horribly if exit returns.  */
  { int *p = 0; *p = 0; }
}
