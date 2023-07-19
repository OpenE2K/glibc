/* Finalizer module for ELF shared C library.  This provides terminating
   null pointer words in the `.ctors' and `.dtors' sections.  */

#ifndef NO_CTORS_DTORS_SECTIONS

/* See Bug #72714 for why alignment should be explicitly specified for objects
   in .{c,d}tors sections when compiling with LCC. Though it's really crucial
   for 32-bit sparc only (see Comment #5), let it be set in GCC-compatible way
   in all our modes.  */

static void (*const __CTOR_END__[1]) (void)
     __attribute__ ((used, section (".ctors")
#ifdef __LCC__
                     , aligned (sizeof (void *))
#endif /* __LCC__  */
                     ))
     = { 0 };
static void (*const __DTOR_END__[1]) (void)
     __attribute__ ((used, section (".dtors")
#ifdef __LCC__
                     , aligned (sizeof (void *))
#endif /* __LCC__  */
                     ))
     = { 0 };
#endif

/* Terminate the frame unwind info section with a 4byte 0 as a sentinel;
   this would be the 'length' field in a real FDE.  */

typedef unsigned int ui32 __attribute__ ((mode (SI)));
static const ui32 __FRAME_END__[1]
  __attribute__ ((used, section (".eh_frame")))
  = { 0 };
