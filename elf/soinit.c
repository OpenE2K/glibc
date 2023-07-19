/* Initializer module for building the ELF shared C library.  This file and
   sofini.c do the work normally done by crtbeginS.o and crtendS.o, to wrap
   the `.ctors' and `.dtors' sections so the lists are terminated, and
   calling those lists of functions.  */

#ifndef NO_CTORS_DTORS_SECTIONS
# include <stdlib.h>

/* See Bug #72714 for why alignment should be explicitly specified for objects
   in .{c,d}tors sections when compiling with LCC. Though it's really crucial
   for 32-bit sparc only (see Comment #5), let it be set in GCC-compatible way
   in all our modes.  */

static void (*const __CTOR_LIST__[1]) (void)
  __attribute__ ((used, section (".ctors")
#ifdef __LCC__
                  , aligned (sizeof (void *))
#endif /* __LCC__  */
                  ))
  = { (void (*) (void)) -1 };
static void (*const __DTOR_LIST__[1]) (void)
  __attribute__ ((used, section (".dtors")
#ifdef __LCC__
                  , aligned (sizeof (void *))
#endif /* __LCC__  */
                  ))
  = { (void (*) (void)) -1 };

static inline void
run_hooks (void (*const list[]) (void))
{
  while (*++list)
    (**list) ();
}

/* This function will be called from _init in init-first.c.  */
void
__libc_global_ctors (void)
{
#if defined __e2k__ && defined __LCC__
  /* Take care of initializing `__cpu_info', required for Bug #89242
     workarounds in LCC atomic builtins to work properly (Bug #90227),
     explicitly since `-llcc' along with `-lgcc' aren't enclosed between
     `so{init,fini}.os' on libc.so linkage line.  */
  __builtin_cpu_init_ext ();
#endif /* __e2k__ && __LCC__  */

  /* Call constructor functions.  */
  run_hooks (__CTOR_LIST__);
}


/* This function becomes the DT_FINI termination function
   for the C library.  */
void
__libc_fini (void)
{
  /* Call destructor functions.  */
  run_hooks (__DTOR_LIST__);
}

void (*_fini_ptr) (void) __attribute__ ((section (".fini_array")))
     = &__libc_fini;
#endif
