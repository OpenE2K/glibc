/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Machine-dependent ELF dynamic relocation inline functions.  e2k version.
   Copyright (C) 1995-2005, 2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

/* This one is required to make it possible to use N_("") macro when emitting
   error notifications via `_dl_signal_error ()' in nested `dl-machine.h'
   files.  */
#include <libintl.h>

static inline void flush_icache_range (unsigned long start, unsigned long end);

#include_next <dl-machine.h>

#ifndef dl_machine_h
#define dl_machine_h

#define ELF_MACHINE_NAME "e2k"

/* Return nonzero iff ELF header is compatible with the running host.  */
static inline int __attribute__ ((unused))
elf_machine_matches_host (const ElfW(Ehdr) *ehdr)
{
  return (ehdr->e_machine == EM_MCST_ELBRUS
          || ehdr->e_machine == EM_E2K_OLD);
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

/* E2k overlaps DT_RELA and DT_PLTREL.  */
#define ELF_MACHINE_PLTREL_OVERLAP 1

#define MAS_ICACHE_LINE_FLUSH	0x17
#define E2K_ICACHE_SET_SIZE	256
#define E2K_ICACHE_SET_MASK	(E2K_ICACHE_SET_SIZE - 1)

static inline void
flush_icache_range (unsigned long start, unsigned long end)
{
  unsigned long addr;

  for (addr = start & ~E2K_ICACHE_SET_MASK;
       addr < ((end + E2K_ICACHE_SET_MASK) & ~E2K_ICACHE_SET_MASK);
       addr += E2K_ICACHE_SET_SIZE)
    {    
      /* E2K_WAIT_ALL; */
      /* Exotic operations like WAIT are no longer supported in asm_inline
         `asm's (see Bug #69048).  */
#pragma no_asm_inline
      asm volatile ("wait \tma_c = 1, fl_c = 1, ld_c = 1, st_c = 1, all_e = 1, all_c = 1");
      /* E2K_WRITE_MAS_D((long) 0x0, addr, MAS_ICACHE_LINE_FLUSH); */
#pragma no_asm_inline
      asm volatile ("std, 2\t0x0, %0, %1, mas = %2" : : "r" ((unsigned long long) 0x0), "r" ((unsigned long long) addr), "i" (MAS_ICACHE_LINE_FLUSH));
      /* E2K_WAIT_ALL; */
#pragma no_asm_inline
      /* Exotic operations like WAIT are no longer supported in asm_inline
         `asm's (see Bug #69048).  */
      asm volatile ("wait \tma_c = 1, fl_c = 1, ld_c = 1, st_c = 1, all_e = 1, all_c = 1");
    }

}

extern ElfW(Addr) _dl_fixup (struct link_map *l, ElfW(Word) reloc_offset);

static inline int __attribute__ ((unused, always_inline))
elf_machine_runtime_setup (struct link_map *l, int lazy, int profile)
{ 
  /* Lazy binding is supported only for modules with DT_E2K_LAZY. */
  if (l->l_info[DT_E2K_LAZY - DT_LOPROC + DT_NUM] != NULL
      && l->l_info[DT_PLTGOT] != NULL)
    {
      ElfW(Addr) *plt = (ElfW(Addr) *) D_PTR (l, l_info[DT_PLTGOT]);
      E2K_RUNTIME_SETUP;

      return lazy;
    }
  else if (l->l_info[DT_E2K_LAZY_GOT - DT_LOPROC + DT_NUM] != NULL
           && l->l_info[DT_PLTGOT] != NULL)
    {
      ElfW(Addr) *got = (ElfW(Addr) *) D_PTR (l, l_info[DT_PLTGOT]);
      ElfW(Addr) fixup_addr = (ElfW(Addr)) _dl_fixup;
      ElfW(Addr) l_addr = (ElfW(Addr)) l;

      got[1] = l_addr;
      got[2] = fixup_addr;

      return lazy;
    }
  else if (l->l_info[DT_E2K_LAZY_DEFECTIVE - DT_LOPROC + DT_NUM] != NULL
           && l->l_info[DT_PLTGOT] != NULL)
    {
      ElfW(Addr) *plt = (ElfW(Addr) *) D_PTR (l, l_info[DT_PLTGOT]);
      E2K_RUNTIME_DEFECTIVE_SETUP;
      /* Do we need to flush icache here at all? Has anyone executed
         any code within PLT range already? In case we do, the whole
         PLT range should be flushed rather than just its header,
         now that Bug #60151 is fixed in runtime.  */
#if 0
      flush_icache_range ((unsigned long) &plt[E2K_RTIME_MIN],
                          (unsigned long) &plt[E2K_RTIME_MAX]);
#endif /* 0  */
      return lazy;
    }
  else if (l->l_info[DT_E2K_LAZY_BUG_75842 - DT_LOPROC + DT_NUM] != NULL
           && l->l_info[DT_PLTGOT] != NULL)
    {
      ElfW(Addr) *plt = (ElfW(Addr) *) D_PTR (l, l_info[DT_PLTGOT]);
      E2K_RUNTIME_BUG_75842_SETUP;
      return lazy;
    }

  return 0;
}

/* This function used to be nested in the original sources of glibc, the reason
   for which was a mystery to me, because it didn't access local variables
   belonging to its callers.  */
static inline void
elf_machine_lazy_rel (struct link_map *map, ElfW(Addr) l_addr,
                      const ElfW(Rela) *reloc, int skip_ifunc,
                      struct r_scope_elem *scope[], const char *strtab)
{
  /* For defective PLT entries everything is done via E2K_RUNTIME_DEFECTIVE
     _SETUP. They require additional conversion and I've no desire to change
     anything related to their support before it's thrown away.  */
  if (map->l_info[DT_E2K_LAZY_DEFECTIVE - DT_LOPROC + DT_NUM] != NULL)
    return;

  /* FIND OUT what's the point in having a separate `l_addr' parameter?
     Why can't we get it from MAP?  */
  ElfW(Addr) *const reloc_addr = (void *) (l_addr + reloc->r_offset);

  if (map->l_info[DT_E2K_LAZY - DT_LOPROC + DT_NUM] != NULL)
    {
      if (ELFXX_R_TYPE (reloc->r_info) == R_E2K_XX_JMP_SLOT)
        {
#if XX == 32
          if (reloc_addr[3] == 0x33333333)
            /* ATTENTION: here `map->l_addr' is employed unlike the code
               fragment a few lines above. . .  */
            reloc_addr[3] = (Elf32_Addr) D_PTR (map, l_info[DT_PLTGOT]);
#else /* XX == 64  */
          if (reloc_addr[1] == 0x3333333333333333ULL)
            {
              /* ATTENTION: here `map->l_addr' is employed unlike the code
                 fragment a few lines above. . .  */
              Elf64_Addr plt_start
		= (Elf64_Addr) D_PTR (map, l_info[DT_PLTGOT]);
              Elf32_Addr *p_plt_start = (Elf32_Addr *) &plt_start;
              Elf64_Addr plt_start_lit = ((((Elf64_Addr) p_plt_start[0]) << 32)
                                          + (Elf64_Addr) p_plt_start[1]);
              reloc_addr[1] = plt_start_lit;
            }
#endif /* XX == 64  */
          else
            /* FIND OUT how this can happen. Is repeated initial "adjustment"
               of a PLT entry harmful at all?  */
            _dl_signal_error (0, map->l_name, NULL,
                              N_("elf_machine_lazy_rel ivoked multiple times "
                                 "for the same PLT entry"));
        }
      else
        _dl_reloc_bad_type (map, ELFW(R_TYPE) (reloc->r_info), 1);
    }
  else if (map->l_info[DT_E2K_LAZY_GOT - DT_LOPROC + DT_NUM] != NULL)
    {
      if (ELFXX_R_TYPE (reloc->r_info) == R_E2K_XX_JMP_SLOT)
        *reloc_addr += l_addr;
      else if (ELFXX_R_TYPE (reloc->r_info) == R_E2K_XX_IRELATIVE)
        {
          /* Note that "lazy processing of R_E2K_XX_IRELATIVE" seems to be a
             misnomer. In fact the job related to this relocation is never
             delegated to `_dl_fixup ()', which is capable of processing one
	     relocation type only being assigned to ELF_MACHINE_JMP_SLOT. For
	     E2K this is `R_E2K_XX_JMP_SLOT'. At the same time processing
	     R_E2K_XX_IRELATIVE doesn't require a lookup through the list of
	     loaded libraries and thus can be easily accomplished at once.

	     With the aforesaid in mind, the final relocation value should be
	     evaluated here. What's the point in placing it to `.rela.plt'
	     then?

	     `SKIP_IFUNC == 1' case corresponds to trace mode, which lets one
	     eliminate the need for calling the resolver.  */
          ElfW(Addr) value = map->l_addr + reloc->r_addend;
          /* What are we going to obtain if SKIP_IFUNC turns out to be TRUE I
             wonder . . . ?  */
          if (__glibc_likely (!skip_ifunc))
            value = ((ElfW(Addr) (*) (void)) value) ();

          *reloc_addr = value;
        }
      else
        _dl_reloc_bad_type (map, ELFW(R_TYPE) (reloc->r_info), 1);
    }
}


/* Fixup a PLT entry to bounce directly to the function at VALUE.  */
static inline ElfW(Addr)
elf_machine_fixup_plt (struct link_map *map,
                       lookup_t t,
		       const ElfW(Sym) *refsym, const ElfW(Sym) *sym,
		       const ElfW(Rela) *reloc,
		       ElfW(Addr) *reloc_addr,
                       ElfW(Addr) value)
{
  /* There is probably no point in verifying RELOC once more, now that this has
     already been done in `elf_machine_lazy_rel', is there?  */

  if (map->l_info[DT_E2K_LAZY - DT_LOPROC + DT_NUM] != NULL
      || map->l_info[DT_E2K_LAZY_DEFECTIVE - DT_LOPROC + DT_NUM] != NULL)
    {
#if XX == 32
      reloc_addr[3] = value;
#else /* XX == 64  */
      Elf64_Addr *reloc_addr64 = (Elf64_Addr *) reloc_addr;
      Elf32_Addr *p_val64 = (Elf32_Addr *) &value;
      Elf64_Addr lit_val = ((((Elf64_Addr) p_val64[0]) << 32)
                            + (Elf64_Addr) p_val64[1]);
      reloc_addr64[1] = lit_val;
#endif /* XX == 64  */

      flush_icache_range ((unsigned long) &reloc_addr[0],
                          (unsigned long) &reloc_addr[6]);
    }
  else if (map->l_info[DT_E2K_LAZY_BUG_75842 - DT_LOPROC + DT_NUM] != NULL)
    E2K_FIXUP_LAZY_BUG_75842_PLT (reloc_addr, value);
  else if (map->l_info[DT_E2K_LAZY_GOT - DT_LOPROC + DT_NUM] != NULL)
      *reloc_addr = value;

  return value;
}



#define ELF_MACHINE_JMP_SLOT	R_E2K_XX_JMP_SLOT
#define ARCH_LA_PLTENTER e2kXX_gnu_pltenter
#define ARCH_LA_PLTEXIT e2kXX_gnu_pltexit

#define ELF_MACHINE_NO_REL      1
#define ELF_MACHINE_NO_RELA     0

#define elf_machine_plt_value(map,reloc,value) value

#define FIX_USER_STACK \
extern  unsigned int _dl_skip_args;                             \
static void                                                     \
_fix_user_stack (long *user_stack)                              \
{                                                               \
  int *pargc;                                                   \
  int i, null_cntr = 0;                                         \
                                                                \
  pargc = (int *) user_stack;                                   \
  *pargc -= _dl_skip_args;                                      \
                                                                \
  /* Shift by _dl_skip_args elements downwards three arrays	\
     adjacent in memory: argv, envp and auxv. When doing so	\
     make use of the fact that the trailing element in each	\
     array is NULL.  */						\
  for (i = 1; ; i++)                                            \
    {                                                           \
      user_stack[i] = user_stack[i + _dl_skip_args];            \
      if (user_stack[i] == 0L)					\
        {                                                       \
          if (++null_cntr == 3)                                 \
            break;                                              \
        }                                                       \
    }                                                           \
                                                                \
  /* In case ld.so is run explicitly _dl_argv is increased in	\
     parallel with the evaluation of _dl_skip_args in _dl_main.	\
     On those platforms where stack pointer can't be adjusted   \
     before passing control to the main executable and argv[],	\
     envp[] and auxv[] need to be shifted (e.g. sparc, ia64,	\
     e2k) it's crucial to revert _dl_argv.  */		        \
  _dl_argv -= _dl_skip_args;                                    \
}


#define RTLD_START 							\
                                                                        \
FIX_USER_STACK                                                          \
static ElfW(Addr) _dl_start (void *arg);				\
                                                                        \
ElfW(Addr)								\
_start2 (void)								\
{									\
  register long *fp asm ("FP");						\
  int argc;								\
  char **argv, **envp;							\
  ElfW(Addr) entry_point;						\
									\
  extern void _dl_init (struct link_map *main_map, int argc, char **argv, char **env); \
                                                                        \
  entry_point = _dl_start (fp);                                         \
  _fix_user_stack (fp);                                                 \
									\
  argc = *fp;								\
  argv = (char**)(fp + 1);						\
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

/* ELF_RTYPE_CLASS_PLT iff TYPE describes relocation of a PLT entry or
   TLS variable, so undefined references should not be allowed to
   define the value.
   ELF_RTYPE_CLASS_NOCOPY iff TYPE should not be allowed to resolve to one
   of the main executable's symbols, as for a COPY reloc.  */
# define elf_machine_type_class(type)                                   \
  ((((type) == R_E2K_XX_JMP_SLOT || (type) == R_E2K_TLS_XX_DTPMOD       \
     || (type) == R_E2K_TLS_XX_DTPREL                                   \
     || (type) == R_E2K_TLS_XX_TPREL) * ELF_RTYPE_CLASS_PLT)            \
   | (((type) == R_E2K_XX_COPY) * ELF_RTYPE_CLASS_COPY)                 \
   | (((type) == R_E2K_XX_ABS) * ELF_RTYPE_CLASS_EXTERN_PROTECTED_DATA))


#endif /* !dl_machine_h */

#ifdef RESOLVE_MAP

#if !defined RTLD_BOOTSTRAP || defined _NDEBUG
#define DL_RELOC_BAD_TYPE(__reloc, __reloc_addr, __value)                \
  default:                                                              \
  _dl_debug_printf("Incorrect reloc type\n");                           \
  _dl_debug_printf("%x\n", (long long) __reloc->r_info);                   \
  _dl_debug_printf("%x\n", (long long) __reloc_addr);                      \
  _dl_debug_printf("%x\n", (long long) __value);                           \
  _dl_debug_printf("----\n");                                           \
  break
#else /* !defined RTLD_BOOTSTRAP || defined _NDEBUG */
#define DL_RELOC_BAD_TYPE(__reloc, __reloc_addr, __value)
#endif /* !define RTLD_BOOTSTRAP || defined _NDEBUG */

/* Perform the relocation specified by RELOC and SYM (which is fully resolved).
   MAP is the object containing the reloc.  */

static void
elf_machine_rela (struct link_map *map, const ElfW(Rela) *reloc,
		  const ElfW(Sym) *sym, const struct r_found_version *version,
		  void *const reloc_addr_arg, int skip_ifunc,
                  struct r_scope_elem *scope[], const char *strtab)
{
  ElfW (Addr) *const reloc_addr = reloc_addr_arg;
  const unsigned long r_type = ELFXX_R_TYPE (reloc->r_info);

#if !defined RTLD_BOOTSTRAP && !defined HAVE_Z_COMBRELOC
  /* This is defined in rtld.c, but nowhere in the static libc.a; make the
     reference weak so static programs can still link.  This declaration
     cannot be done when compiling rtld.c (i.e.  #ifdef RTLD_BOOTSTRAP)
     because rtld.c contains the common defn for _dl_rtld_map, which is
     incompatible with a weak decl in the same file.  */
  weak_extern (_dl_rtld_map);
#endif

  if (r_type == R_E2K_XX_RELATIVE)
    {
#ifndef RTLD_BOOTSTRAP
      /* Already done in rtld itself.  */
      if (map != &GL (dl_rtld_map))
#endif /* RTLD_BOOTSTRAP  */
        *reloc_addr = map->l_addr + reloc->r_addend;
    }
  /* This one should be never met in ld.so itself but in poorly
     linked libraries only. */
  else if (ELFXX_R_TYPE (reloc->r_info) == R_E2K_64_RELATIVE_LIT)
    {
      /* Should not be met in rtld itself. */
#ifndef RTLD_BOOTSTRAP
      if (map != &GL(dl_rtld_map))
#endif /* RTLD_BOOTSTRAP  */
        {
          Elf64_Addr value = map->l_addr + reloc->r_addend; 
          *((Elf32_Addr *) reloc_addr) = (Elf32_Addr) (value >> 32);
          *((Elf32_Addr *) reloc_addr + 1) = (Elf32_Addr) (value & 0xffffffff);
        }
    }
  else if (ELFXX_R_TYPE (reloc->r_info) != R_E2K_NONE)
    {
#ifndef RTLD_BOOTSTRAP
      const ElfW(Sym) *const refsym = sym;
#endif /* RTLD_BOOTSTRAP  */
      ElfW(Addr) value;
      struct link_map *sym_map;

      if (sym->st_shndx != SHN_UNDEF
          && ELFXX_ST_BIND (sym->st_info) == STB_LOCAL)
        {
          sym_map = map;
          value = map->l_addr;
        }
      else
        {
          sym_map =  RESOLVE_MAP (&sym, version,
                                  ELFXX_R_TYPE (reloc->r_info));
          value = sym_map == NULL ? 0 : sym_map->l_addr;
          if (sym)
            {
              value += sym->st_value;
              if (__builtin_expect (sym->st_shndx != SHN_UNDEF, 1)
                  && __builtin_expect ((ELFW(ST_TYPE) (sym->st_info)
                                        == STT_GNU_IFUNC),
                                       0)
                  && __builtin_expect (!skip_ifunc, 1))
                value = ((ElfW(Addr) (*) (void)) value) ();
            }
        }
      /* Assume copy relocs have zero addend.  */
      value += reloc->r_addend;

      switch (ELFXX_R_TYPE (reloc->r_info))
        {
        case R_E2K_32_SIZE:
          *((Elf32_Addr *) reloc_addr) = sym->st_size + reloc->r_addend;
          break;
        case R_E2K_64_SIZE:
          *((Elf64_Addr *) reloc_addr) = sym->st_size + reloc->r_addend;
          break;

        case R_E2K_32_ABS:
          /* FIXME: allow for "empty" entries filled in with zeroes in the
	     relocation table for backward compatibility. They may have
	     appeared in the past in case LD allocated more space for dynamic
	     relocations than needed. Nowadays excessive relocations are tagged
	     with R_E2K_NONE. Note that unlike most other architectures on E2K
	     R_E2K_NONE != 0 because 0 has improvidently been reserved for
	     R_E2K_32_ABS from the very beginning.  */
          if (reloc->r_offset)
            *reloc_addr = value;
          break;

#ifdef dl_machine_64_h
          /* In case a 64-bit library is being built the macro mentioned
             above has been defined in the internal <dl-machine.h>.  */
        case R_E2K_64_ABS:
          *reloc_addr = value;
          break;
        case R_E2K_64_ABS_LIT:
          *((Elf32_Addr *) reloc_addr) = (Elf32_Addr) (value >> 32);
          *((Elf32_Addr *) reloc_addr + 1)
            = (Elf32_Addr) (value & 0xffffffff);
          break;
#endif /* dl_machine_64_h  */

        case R_E2K_XX_JMP_SLOT:
          if (map->l_info[DT_E2K_LAZY_GOT - DT_LOPROC + DT_NUM] != NULL
              || map->l_info[DT_E2K_LAZY_BUG_75842 - DT_LOPROC + DT_NUM] != NULL
              || map->l_info[DT_E2K_LAZY - DT_LOPROC + DT_NUM] != NULL
              || (map->l_info[DT_E2K_LAZY_DEFECTIVE - DT_LOPROC + DT_NUM]
                  != NULL))
            elf_machine_fixup_plt (map, NULL, NULL, NULL, reloc, reloc_addr,
				   value);
          else
            E2K_FIXUP_PLT (reloc_addr, value);
          break;

#ifndef RTLD_BOOTSTRAP
        case R_E2K_XX_COPY:
          if (sym == NULL)
            /* This can happen in trace mode if an object could
               not be found.  */
            break; 
          if (sym->st_size > refsym->st_size
              || (GLRO (dl_verbose) && sym->st_size < refsym->st_size))
            {
              extern char **_dl_argv;
              const char *strtab;

              strtab = (const void *) D_PTR (map, l_info[DT_STRTAB]);
              _dl_error_printf
                ("%s: Symbol `%s' has different size in shared object, "
                 "consider re-linking\n",
                 _dl_argv[0] ?: "<program name unknown>",
                 strtab + refsym->st_name);
            }
          memcpy (reloc_addr, (void *) value,
                  MIN (sym->st_size, refsym->st_size));
          break;

        case R_E2K_TLS_XX_DTPMOD:
          *reloc_addr = sym_map->l_tls_modid;
          break;
        case R_E2K_TLS_XX_DTPREL:
          *reloc_addr = value - sym_map->l_addr;
          break;
        case R_E2K_TLS_XX_TPREL:
          CHECK_STATIC_TLS (map, sym_map);
          *reloc_addr = value + sym_map->l_tls_offset - sym_map->l_addr;
          break;
        case R_E2K_XX_IRELATIVE:
          /* This presumably duplicates the related activity in `elf_machine_
	     lazy_rel ()' in LD_BIND_NOW case. FIXME: SKIP_IFUNC case isn't
	     considered here by analogy with i386, which is probably a bug.
	     Note that it's taken into account above for non-local ifuncs. By
	     the way, can't we find ourselves here for a non-local ifunc and
	     thus end up calling its resolver more than once?  */
          value = map->l_addr + reloc->r_addend;
          value = ((ElfW(Addr) (*) (void)) value) ();
          *reloc_addr = value;
          break;
#else /* RTLD_BOOTSTRAP  */
        case R_E2K_TLS_XX_DTPMOD:
          *reloc_addr = 1;
          break;
#endif /* RTLD_BOOTSTRAP  */
        default:
          _dl_reloc_bad_type (map, ELFW(R_TYPE) (reloc->r_info), 0);
        }
    }
}

static void __attribute__ ((unused))
elf_machine_rela_relative (ElfW(Addr) l_addr, const ElfW(Rela) *reloc,
			   void *const reloc_addr_arg)
{
  /* It should be impossible to find myself here until I implement grouping
     of relative relocs within an ELF file along with DT_RELACOUNT.  */
  ((ElfW(Addr) *)0)[0] = l_addr + reloc->r_addend;
}


#endif /* RESOLVE_MAP */
