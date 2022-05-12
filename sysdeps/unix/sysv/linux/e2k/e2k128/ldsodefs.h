#ifndef _LINUX_E2K_E2K128_LDSODEFS_H
#define _LINUX_E2K_E2K128_LDSODEFS_H	1

#define ELF_FUNCTION_PTR_IS_SPECIAL

# define DL_LOOKUP_ADDRESS(addr) ((ElfW(Addr)) (addr))

#define DL_CALL_DT_INIT(map, start, argc, argv, env)		\
  {								\
  /* Note that these idiots in `call_init ()' invoked us with a	\
     bogus address in START as if _init () resided in the data	\
     segment. Fortunately, it can be used to extract the valid	\
     offset of this function in the text segment.  */		\
    init_t init = (init_t) get_pl (map, start - map->l_addr);	\
    init (argc, argv, env);					\
  }

#define DL_CALL_DT_FINI(map, start)				\
  {								\
    fini_t fini = (fini_t) get_pl (map, start - map->l_addr);	\
    fini ();							\
  }

#define VALID_ELF_FLAGS(flg) (((flg) & EF_E2K_PM) != 0)

#include_next <ldsodefs.h>

static unsigned int get_offset (struct link_map *map, unsigned int off);

static __attribute__ ((unused)) int
dl_addr_sym_match(struct link_map *l, const ElfW(Sym) *sym,
		  const ElfW(Sym) *matchsym, const ElfW(Addr) addr)
{
  int i;
  int addr_in_cud = 0;
  int sym_in_cud = 0;
  ElfW(Addr) base = (ElfW(Addr)) l->l_gd;

  if (addr >= l->l_text_start && addr < l->l_text_end)
    {
      addr_in_cud = 1;
      base = l->l_code_addr;
    }
  else if (addr < l->l_data_start || addr >= l->l_data_end)
    return 0;

  for (i = 0; i < l->l_phnum; i++)
    {
      if (l->l_phdr[i].p_type == PT_LOAD
	  && sym->st_value >= l->l_phdr[i].p_vaddr
	  && (sym->st_shndx == SHN_UNDEF
	      || sym->st_size == 0
	      || (sym->st_value + sym->st_size
		  <= l->l_phdr[i].p_vaddr + l->l_phdr[i].p_memsz)))
	{
	  if (l->l_phdr[i].p_flags & PF_X)
	    sym_in_cud = 1;
	  break;
	}
    }

  /* SYM doesn't belong to any of the module's PT_LOAD Program Headers.  */
  if (i == l->l_phnum)
    return 0;

  /* Whereas one of ADDR and SYM belongs to GD, another one belongs to CUD.  */ 
  if (addr_in_cud != sym_in_cud)
    return 0;

  unsigned int sym_offset = get_offset (l, sym->st_value);
  return (addr >= base + sym_offset
	  && (((sym->st_shndx == SHN_UNDEF || sym->st_size == 0)
	       && addr == base + sym_offset)
	      || addr < base + sym_offset + sym->st_size)
	  && (matchsym == NULL
	      /* Hopefully the comparison of `st_value's of symbols belonging
		 simultaneously to either CUD or GD makes sense even in packed
		 ELFs as the same relation is expected to hold for matching
		 {CUD,GD} symbols' runtime offsets.  */
	      || matchsym->st_value < sym->st_value));
}

#undef DL_ADDR_SYM_MATCH

#define DL_ADDR_SYM_MATCH(L, SYM, MATCHSYM, ADDR) \
  dl_addr_sym_match (L, SYM, MATCHSYM, ADDR)

#if 0
/* The only difference between this macro and the one provided by `sysdeps/
   generic/ldsodefs.h' is that `l_addr' in the latter is replaced with
   `l_code_addr'. FIXME: this is obviously wrong if either ADDR or SYM belongs
   to GD. As for runtime(!) ADDR, this can be obviously checked by means of
   comparing it against CUD and GD boundaries. But what about SYM, taking into
   account that code and data sections may alternate in ELF address space?
   Should one rely on ELF symbol type to determine that? Isn't the use of
   `l_phdr[]' a more reliable (though a slower) option?  */

#define DL_ADDR_SYM_MATCH(L, SYM, MATCHSYM, ADDR) \
  ((ADDR) >= (L)->l_code_addr + (SYM)->st_value				\
   && ((((SYM)->st_shndx == SHN_UNDEF || (SYM)->st_size == 0)		\
	&& (ADDR) == (L)->l_code_addr + (SYM)->st_value)		\
       || (ADDR) < (L)->l_code_addr + (SYM)->st_value + (SYM)->st_size)	\
   && ((MATCHSYM) == NULL || (MATCHSYM)->st_value < (SYM)->st_value))

#endif /* 0  */

static void
fill_range_num (struct link_map *map)
{
  const ElfW(Phdr) *phdr;
  size_t i;

  for (phdr = map->l_phdr, i = 0; phdr < &map->l_phdr[map->l_phnum]; ++phdr) 
	{
	  if (phdr->p_type == PT_LOAD)
	    i++;
	}

  map->range_num = i;
}

static void
fill_ranges (struct link_map *map, void *p)
{
  const ElfW(Phdr) *phdr;
  size_t i;
  struct range *ranges = (struct range *) p;

  for (phdr = map->l_phdr, i = 0;
       (phdr < &map->l_phdr[map->l_phnum]
	/* Just to be on the safe side.  */
	&& i < map->range_num);
       ++phdr)	
    {
      if (phdr->p_type == PT_LOAD)
	{
	  ranges[i].min = phdr->p_vaddr;
	  ranges[i].max = phdr->p_vaddr + phdr->p_memsz;
	  ranges[i].align = phdr->p_align;
	  ranges[i].in_cud = (phdr->p_flags & PF_X) ? 1 : 0;
	  i++;
	}
    }

  /* Stupidly sort the obtained ranges. I believe that qsort () may be
     unavailable in context of ld.so  */
  for (i = 0; i < map->range_num - 1; i++)
    {
      size_t j;
      for (j = i + 1; j < map->range_num; j++)
	{
	  if (ranges[j].min < ranges[i].min)
	    {
	      struct range tmp = ranges[i];
	      ranges[i] = ranges[j];
	      ranges[j] = tmp;
	    }
	}
    }

  uintptr_t max_page_aligned = 0;
  uintptr_t cud_size = 0, gd_size = 0;

  for (i = 0; i < map->range_num; i++)
    {
      uintptr_t min_page_aligned = ranges[i].min & ~(ranges[i].align - 1);
      uintptr_t b, r, pos;

      /* MAX_PAGE_ALIGNED corresponds to the preceding segment in this
	 comparison, of course.  */
      if (min_page_aligned < max_page_aligned)
	{
	  /* The Kernel should have failed to load this executable
	     in such a case.  */
	}

      max_page_aligned = (ranges[i].max + 0xfffULL) & ~(0xfffULL);


      if (ranges[i].in_cud)
	pos = cud_size;
      else
	pos = gd_size;

      pos = (pos + 0xfffULL) & ~0xfffULL;
      r = ranges[i].min & (ranges[i].align - 1);
      b = (pos + ranges[i].align - (r + 1)) / ranges[i].align;
      pos = b * ranges[i].align + r;

      ranges[i].delta = pos - ranges[i].min;
      pos += ranges[i].max - ranges[i].min;

      if (ranges[i].in_cud)
	cud_size = pos;
      else
	gd_size = pos;
    }
}

static void
see_if_packed (struct link_map *map)
{
  const ElfW(Phdr) *phdr;

  if (map->l_phdr == NULL)
    {
      /* An unbelievable case.  */
      map->packed = PACKED_LEGACY;
      return;
    }

  for (phdr = map->l_phdr; phdr < &map->l_phdr[map->l_phnum]; ++phdr)
    {
      if (phdr->p_offset == 0 && !(phdr->p_flags & PF_X))
	{
	  /* This should be the "first" (i.e. the one at 0 offset in ELF)
	     segment containing Program Headers.  */
	  uintptr_t phdr_offset;
	  phdr_offset = (uintptr_t) map->l_phdr - (uintptr_t) map->l_gd;
	  /* In non-packed case Program Headers would be mapped at offset
	     within GD equal to their "address" in ELF, i.e. the one
	     exceeding p_vaddr of the containing segment. This condition could
	     probably result in a wrong conclusion if p_vaddr of the first
	     DATA segment turned out to be 0, which is very unlikely as it's
	     typically the TEXT segment that comes first with `p_vaddr > 0'
	     in ELF address space. It could also mislead us if the offset to
	     Program Headers in ELF proved to be greater than p_vaddr of the
	     containing segment. FIXME(?): it would be much more reliable and
	     straightforward to check for EF_E2K_PACK_SEGMENTS in e_flags,
	     however, there seems to be no good way of obtaining ELF header
	     of the main executable loaded entirely by the Kernel.  */
	  map->packed = (phdr_offset < phdr->p_vaddr
			 ? PACKED_PACKED
			 : PACKED_LEGACY);

	  break;
	}
    }

  /* If we turned out to be incapable of determining if MAP is packed, stupidly
     believe that it is not.  */
  if (map->packed == PACKED_UNKNOWN)
    map->packed = PACKED_LEGACY;

  if (map->packed == PACKED_LEGACY)
    return;

  fill_range_num (map);

  /* Currently MAP can store up to 8 sorted address ranges matching
     PT_LOAD ELF segments. In a very unlikely case when this number
     exceeds 8, the ranges need to be temporarily reobtained during
     each evaluation of the packed offset.   */
  if (map->range_num > 8)
    return;

  fill_ranges (map, map->ranges);
}

static unsigned int
get_packed_offset_in_ranges (struct link_map *map, struct range *ranges,
			     unsigned int off, int in_cud)
{
  size_t i;
  if (! in_cud)
    {
      for (i = 0; i < map->range_num; i++)
	{
	  if (off >= ranges[i].min && off < ranges[i].max)
	    return off + ranges[i].delta;
	}
    }
  else /* in_cud  */
    {
      for (i = 0; i < map->range_num; i++)
	{
	  if (ranges[i].in_cud
	      && off >= (ranges[i].min & -((uintptr_t) 0x1000))
	      && off <= ((ranges[i].max + 0xfff) & -((uintptr_t) 0x1000)))
	    return off + ranges[i].delta;
	}
    }

  /* FIXME: stupidly return OFF if no matching range was found.  */
  return off;
}

static unsigned int
get_packed_offset_slow (struct link_map *map, unsigned int off, int in_cud)
{
  struct range ranges[map->range_num];

  fill_ranges (map, ranges);
  return get_packed_offset_in_ranges (map, ranges, off, in_cud);
}

static unsigned int
get_packed_offset (struct link_map *map, unsigned int off, int in_cud)
{
  if (map->range_num <= 8)
    return get_packed_offset_in_ranges (map, map->ranges, off, in_cud);

  return get_packed_offset_slow (map, off, in_cud);
}

static __attribute__ ((unused)) unsigned int
get_offset_ex (struct link_map *map, unsigned int off, int in_cud)
{
  if (map->packed == PACKED_UNKNOWN)
    see_if_packed (map);

  if (map->packed == PACKED_LEGACY)
    return off;

  return get_packed_offset (map, off, in_cud);
}

static __attribute__ ((unused)) unsigned int
get_offset (struct link_map *map, unsigned int off)
{
  return get_offset_ex (map, off, 0);
}


static __attribute__ ((unused)) unsigned int *
get_uint_slot (struct link_map *map, unsigned int off)
{
  unsigned int *res;

  off = get_offset (map, off);
  res = __builtin_e2k_create_ap_subarray (map->l_gd, off, 4);
  return res;
}

static __attribute__ ((unused)) void **
get_ap_slot (struct link_map *map, unsigned int off)
{
  void **res;

  off = get_offset (map, off);
  res = __builtin_e2k_create_ap_subarray (map->l_gd, off, 16);
  return res;
}

static __attribute__ ((unused)) void
(** get_pl_slot (struct link_map *map, unsigned int off)) (void)
{
  void (** res) (void);
  off = get_offset (map, off);
  res = __builtin_e2k_create_ap_subarray (map->l_gd, off,
					  sizeof (void (*) (void)));
  return res;
}


static __attribute__ ((unused)) void *
get_ap (struct link_map *map, unsigned int off,
	unsigned int addend, unsigned int size)
{
  char *res;
  off = get_offset (map, off);
  res = __builtin_e2k_create_ap_subarray (map->l_gd, off, size);
  return &res[addend];
}

static __attribute__ ((unused)) void
(* get_pl (struct link_map *map, unsigned int off)) (void)
{
  unsigned int i;
  unsigned int size = map->l_info[DT_E2K_EXPORT_PLSZ - DT_LOPROC
				  + DT_NUM]->d_un.d_val;
  void (**pl) (void) = get_ap (map,
			       (map->l_info[DT_E2K_EXPORT_PL
					    - DT_LOPROC + DT_NUM]->d_un.d_ptr),
			       0, size);
  unsigned long addr = map->l_code_addr + get_offset (map, off);
  /* Take into account that upstream e2k-linux-ld always reserves 16 bytes per
     function pointer in `.data.export_pl' no matter whether it's 16 or 8 bytes
     long in fact.  */
  unsigned int n_slots = size / 16;
  unsigned int scale = 16 / sizeof (void (*) (void));

  for (i = 0; i < n_slots; i++)
    {
      if ((unsigned long) pl[scale * i] == addr)
	return pl[scale * i];
    }
  
  return NULL;
}

static __attribute__ ((unused)) void *
dl_symbol_address (struct link_map *l, const ElfW(Sym) *sym)
{
  int i;
  int sym_in_cud = 0;

  for (i = 0; i < l->l_phnum; i++)
    {
      if (l->l_phdr[i].p_type == PT_LOAD
	  && sym->st_value >= l->l_phdr[i].p_vaddr
	  && (sym->st_shndx == SHN_UNDEF
	      || sym->st_size == 0
	      || (sym->st_value + sym->st_size
		  <= l->l_phdr[i].p_vaddr + l->l_phdr[i].p_memsz)))
	{
	  if (l->l_phdr[i].p_flags & PF_X)
	    sym_in_cud = 1;
	  break;
	}
    }

  if (i == l->l_phnum)
    return NULL;

  if (sym_in_cud)
    return (void *) get_pl (l, sym->st_value);

  return get_ap (l, sym->st_value, 0, sym->st_size);
}

# define DL_SYMBOL_ADDRESS(map, ref) \
  dl_symbol_address (map, ref)

#endif /* _LINUX_E2K_E2K128_LDSODEFS_H  */
