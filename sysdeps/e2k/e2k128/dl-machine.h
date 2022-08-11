#ifndef dl_machine_128_h
#define dl_machine_128_h

typedef struct
{
  void (* selfinit) (void);
  char *gd;
} mdd_t;


#define elf_machine_plt_value(map,reloc,value) value

/* ELF_RTYPE_CLASS_PLT iff TYPE describes relocation of a PLT entry or
   TLS variable, so undefined references should not be allowed to
   define the value.
   ELF_RTYPE_CLASS_NOCOPY iff TYPE should not be allowed to resolve to one
   of the main executable's symbols, as for a COPY reloc.  */
# define elf_machine_type_class(type)					\
  ((((type) == R_E2K_32_JMP_SLOT					\
     || (type) == R_E2K_TLS_32_DTPMOD					\
     || (type) == R_E2K_TLS_32_DTPREL					\
     || (type) == R_E2K_TLS_32_TPREL)					\
    * ELF_RTYPE_CLASS_PLT)						\
   | (((type) == R_E2K_32_COPY) * ELF_RTYPE_CLASS_COPY)                 \
   | (((type) == R_E2K_AP || (type) == R_E2K_PL)			\
      * ELF_RTYPE_CLASS_EXTERN_PROTECTED_DATA))



/* Return nonzero iff ELF header is compatible with the running host.  */
static inline int __attribute__ ((unused))
elf_machine_matches_host (const ElfW(Ehdr) *ehdr)
{
  /* There's no point in supporting EM_E2K_OLD in Protected Mode glibc (see
     also a comment before E2K_OSABI).  */
  return ehdr->e_machine == EM_MCST_ELBRUS;
}

/* Return the link-time address of _DYNAMIC.  Conveniently, this is the
   first element of the GOT, a special entry that is never relocated.  */
static inline ElfW(Addr) __attribute__ ((unused, const))
elf_machine_dynamic (void)
{
  /* This produces a GOTOFF reloc that resolves to zero at link time, so in
     fact just loads from the GOT register directly.  By doing it without
     an asm we can let the compiler choose any register.  */
  extern const ElfW(Addr) _GLOBAL_OFFSET_TABLE_[] attribute_hidden;
  return _GLOBAL_OFFSET_TABLE_[0];
}

/* Return the run-time load address of the shared object.  */
static inline ElfW(Addr) __attribute__ ((unused))
elf_machine_load_address (void)
{
  /* Compute the difference between the runtime address of _DYNAMIC as seen
     by a GOTOFF reference, and the link-time address found in the special
     unrelocated first GOT entry.  */
  extern ElfW(Dyn) bygotoff[] asm ("_DYNAMIC") attribute_hidden;
  return (ElfW(Addr)) &bygotoff - elf_machine_dynamic ();
}


/* FIXME: "semantics in MCST sense" is actually irrelevant to Protected Mode
   glibc since we have never had and will never have EM_E2K_OLD PM ELFs
   supported in it. I quite formally define it to the latest value introduced
   in 2008 to mark PM ELFs. Recall that prior to that 3 had been used to mark
   ancient PM ELFs lacking CXX hardware support and 4 to mark those ones
   enjoying it.  */
#define E2K_OSABI       5

#define ELF_MACHINE_JMP_SLOT	R_E2K_32_JMP_SLOT

#if defined __ptr128_new_abi__
# define ARCH_LA_PLTENTER e2k64_gnu_pltenter
# define ARCH_LA_PLTEXIT e2k64_gnu_pltexit
#else /* ! defined __ptr128_new_abi__  */
# define ARCH_LA_PLTENTER e2k32_gnu_pltenter
# define ARCH_LA_PLTEXIT e2k32_gnu_pltexit
#endif /* ! defined __ptr128_new_abi__  */

/* Make sure that REL-specific stuff isn't referenced from the generic part of
   ld.so.  */
#define ELF_MACHINE_NO_REL      1
#define ELF_MACHINE_NO_RELA     0

/* Prevent `d_ptr's in `.dynamic' section from being adjusted in a pointless
   way from the point of view of Protected Mode.  */
#define DL_RO_DYN_SECTION 1


#define CPU_INIT                                \
  __builtin_cpu_init_ext ()

#define FIX_USER_STACK							\
  extern  unsigned int _dl_skip_args;					\
  static void								\
  _fix_user_stack (void **user_stack)					\
  {									\
    int *pargc;								\
    unsigned int *auxv;							\
    int i, null_cntr = 0;						\
									\
    pargc = (int *) user_stack;						\
    *pargc -= _dl_skip_args;						\
									\
    /* Shift by `_dl_skip_args' downwards the elements of three adjacent in memory \
       vectors: `{argv,envp,auxv}[]'. While the former two vectors contain pointers \
       and are terminated with `NULL's, . . .  */			\
    for (i = 1; ; i++)							\
      {									\
	user_stack[i] = user_stack[i + _dl_skip_args];			\
	if (user_stack[i] == NULL)					\
	  {								\
	    if (++null_cntr == 2)					\
	      break;							\
	  }								\
      }									\
									\
    /* . . . the latter consists of `Elf32_auxv_t' structs		\
       with the trailing one having `a_type == AT_NULL'.  */		\
    auxv = (unsigned int *) &user_stack[i + 1];				\
    for (i = 0; ; i += 2)						\
      {									\
	/* Take into account that both `a_type' and `a_val' are 4 bytes long \
	   which is four times shorter than the size of a pointer in PM.  */ \
	auxv[i] = auxv[i + 4 * _dl_skip_args];				\
	auxv[i + 1] = auxv[i + 1 + 4 * _dl_skip_args];			\
	/* Check for `a_type == AT_NULL.  */				\
	if (auxv[i] == 0)						\
	  break;							\
      }									\
									\
    /* When ld.so is executed explicitly, not only does `dl_main ()' count \
       `_dl_skip_args', but it also increases `_dl_argv' in parallel. On those \
       architectures (sparc, ia64, e2k) where it's not possible to adjust the \
       value of the stack pointer when transferring control to the main \
       executable, one should restore the initial value of `_dl_argv' after \
       having shifted the contents  of `{argv,envp,auxv}[]' arrays.  */	\
    _dl_argv -= _dl_skip_args;						\
  }



#define RTLD_START 							\
                                                                        \
FIX_USER_STACK                                                          \
 static void (* _dl_start (void *arg)) (void);				\
                                                                        \
void (* _start2 (void)) (void)						\
{									\
  register void **fp asm ("%r0");					\
  int argc;								\
  char **argv, **envp;							\
  void (* entry_point) (void);						\
									\
  extern void __selfinit (void) __attribute__ ((visibility ("hidden"))); \
  extern void _dl_init (struct link_map *main_map, int argc, char **argv, char **env); \
									\
  __selfinit ();							\
									\
  fp +=1;								\
                                                                        \
  CPU_INIT;                                                             \
                                                                        \
  entry_point = _dl_start (fp);                                         \
									\
  /* Presumably the Kernel loader is to be fixed to make AP describing	\
     the stack area containing `arg{c,v[]},envp[],auxv[]' writable	\
     to make this work.  */						\
  _fix_user_stack (fp);							\
									\
  argc = *((int *) fp);							\
  argv = (char **)(fp + 1);						\
  envp = argv + argc + 1;						\
									\
  /* For most other architectures _dl_init( *_rtld_local,...) call is	\
     performed in assembler here. Do the same for E2K in C.  */		\
  _dl_init (_rtld_local._dl_ns[0]._ns_loaded, argc, argv, envp);	\
									\
  return entry_point;							\
}									\
									\
 RTLD_START_EARLY

#define RTLD_START_EARLY                                                \
  __asm__ ( "\n"                                                        \
            ".text\n"                                                   \
            ".global $_start\n"                                         \
            ".type $_start, @function\n"                                \
            ".align 32\n"                                               \
            "$_start:\n"                                                \
            "{\n"                                                       \
            "  setwd wsz = 0x8, nfx = 0x0\n"                            \
	    "  movtq %r0, %r8\n"						\
            "  disp %ctpr1, $_start2\n"                                 \
            "}\n"                                                       \
            "{\n"                                                       \
            "  call %ctpr1, wbs = 0x4\n"                                \
            "  adds,1 0x0, 0x0, %r1\n"                                  \
            "}\n"                                                       \
            "{\n"                                                       \
            "  movtd,0 %dr8, %ctpr1\n"                                  \
	    "}\n"							\
	    "{\n"							\
            "  movtq %r0, %r8\n"					\
            "}\n"                                                       \
	    "{\n"							\
            "  ldgdq,3 0x0, _dl_fini@PL_GOT, %qr10\n"			\
            "}\n"                                                       \
            "call %ctpr1, wbs = 0x4\n"                                  \
            "stgdw,2 0x0, 0x0, %r1\n" );


extern pfn _dl_fixup (struct link_map *l, ElfW(Word) reloc_offset);

static inline int __attribute__ ((unused, always_inline))
elf_machine_runtime_setup (struct link_map *l, int lazy, int profile)
{
  if (l->l_info[DT_E2K_REAL_PLTGOT - DT_LOPROC + DT_NUM] != NULL)
    {
      void *got = (void *) D_PTR (l, l_info[DT_E2K_REAL_PLTGOT
					    - DT_LOPROC + DT_NUM]);
      
      ((struct link_map **) got)[1] = l;
      ((typeof (&_dl_fixup) *) got) [2] = &_dl_fixup;
    }

  /* FIXME: is it crucial to return zero if no `DT_E2K_REAL_PLTGOT' is present
     in `link_map'? Presumably it makes little difference if the library
     contains no dynamic calls.  */
  return lazy;
}

/* Fixup a PLT entry to bounce directly to the function at VALUE.  */
static inline pfn
elf_machine_fixup_plt (struct link_map *map,
                       lookup_t t,
		       const ElfW(Sym) *refsym, const ElfW(Sym) *sym,
		       const ElfW(Rela) *reloc,
		       ElfW(Addr) *reloc_addr,
                       pfn value)
{
  *((pfn *) reloc_addr) = value;
  return value;
}



#endif /* dl_machine_128_h  */


#if defined RESOLVE_MAP

static inline void
elf_machine_lazy_rel (struct link_map *map, ElfW(Addr) l_addr,
                      const ElfW(Rela) *reloc, int skip_ifunc,
                      struct r_scope_elem *scope[], const char *strtab)
{
  /* Unlike ordinary modes in which .got.plt entries are lazily adjusted to
     run-time addresses of the corresponding secondary PLT entries by means of
     `+= l_addr', in PM this can't be obviously accomplished this way. It's
     `__selfinit ()' which is responsible for performing this job among other
     things.  */
}


/* Perform the relocation specified by RELOC and SYM (which is fully resolved).
   MAP is the object containing the reloc.  */

static void
elf_machine_rela (struct link_map *map, const ElfW(Rela) *reloc,
		  const ElfW(Sym) *sym, const struct r_found_version *version,
		  void *const reloc_addr_arg, int skip_ifunc,
                  struct r_scope_elem *scope[], const char *strtab)
{
  ElfW(Addr) value;
  struct link_map *sym_map;
  const unsigned long r_type = ELF32_R_TYPE (reloc->r_info);

  /* Currently it's required only when processing the copy relocation
     below.  */
  const ElfW(Sym) *dst_sym = sym;

  if (sym->st_shndx != SHN_UNDEF
      && ELF32_ST_BIND (sym->st_info) == STB_LOCAL)
    {
      sym_map = map;
      value = map->l_addr;
    }
  else
    {
      sym_map =  RESOLVE_MAP (&sym, version,
			      ELF32_R_TYPE (reloc->r_info));
      value = sym_map == NULL ? 0 : sym_map->l_addr;
      if (sym)
	value += sym->st_value;
    }

  if (r_type == R_E2K_AP)
    *get_ap_slot (map, reloc->r_offset)
      = (sym
	 ? get_ap (sym_map, sym->st_value, reloc->r_addend, sym->st_size)
	 : (void *) 0);
  else if (r_type == R_E2K_PL || r_type == R_E2K_32_JMP_SLOT)
    *get_pl_slot (map, reloc->r_offset)
      = (sym
	 ? get_pl (sym_map, sym->st_value)
	 : (void (*) (void)) 0);
  else if (r_type == R_E2K_32_COPY)
    {
      unsigned int st_size = (sym->st_size < dst_sym->st_size
			      ? sym->st_size
			      : dst_sym->st_size);
      memcpy (get_ap (map, reloc->r_offset, 0, st_size),
	      get_ap (sym_map, sym->st_value, 0, st_size), st_size);
    }
  else if (r_type == R_E2K_TLS_32_DTPMOD)
    /* Note that `l_tls_modid' is `size_t' and therefore 64 bits long, whereas
       this relocation is 32-bit. Once again, what's the point in having 64-bit
       long in PM?  */
    *get_uint_slot (map, reloc->r_offset)
      = (unsigned int) sym_map->l_tls_modid;
  else if (r_type == R_E2K_TLS_32_DTPREL)
    *get_uint_slot (map, reloc->r_offset) = (value + reloc->r_addend
					     - sym_map->l_addr);
  else if (r_type == R_E2K_TLS_32_TPREL)
    {
      CHECK_STATIC_TLS (map, sym_map);
      *get_uint_slot (map, reloc->r_offset)
	= (value + reloc->r_addend + sym_map->l_tls_offset - sym_map->l_addr);
    }
}

static void __attribute__ ((unused))
elf_machine_rela_relative (ElfW(Addr) l_addr, const ElfW(Rela) *reloc,
			   void *const reloc_addr_arg)
{
}


/* Return the PL of the entry point. */
#define ELF_MACHINE_START_ADDRESS(map, start)			\
  ({								\
    void (*pl) (void);						\
    pl = get_pl (map, start - map->l_code_addr);		\
    pl;								\
  })


#endif /* defined RESOLVE_MAP  */
