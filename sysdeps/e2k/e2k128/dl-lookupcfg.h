#ifndef DL_LOOKUPCFG_E2K128
#define DL_LOOKUPCFG_E2K128

#include_next <dl-lookupcfg.h>

typedef void (* pfn) (void);


#undef DL_FIXUP_VALUE_TYPE
#define DL_FIXUP_VALUE_TYPE pfn

#undef DL_FIXUP_MAKE_VALUE
#define DL_FIXUP_MAKE_VALUE(map, addr) \
  ((pfn) get_pl (map, (addr) - map->l_addr))

/* First they convert a function pointer to `Elf32_Addr' via `DL_FIXUP_VALUE_
   _ADDR()' and store it into `sym.st_value', then `sym.st_value' is converted
   back to a function pointer by means of `DL_FIXUP_ADDR_VALUE' after which
   `DL_FIXUP_VALUE_CODE_ADDR ()' is used to get the function's PC. All this may
   make sense for IA-64 but looks ugly in Protected Mode: the pointer to a
   function obtained via `DL_FIXUP_ADDR_VALUE ()' isn't usable for anything but
   getting the related PC in fact!  */
#undef DL_FIXUP_VALUE_CODE_ADDR
#define DL_FIXUP_VALUE_CODE_ADDR(value) ((ElfW(Addr)) value)

#undef DL_FIXUP_VALUE_ADDR
#define DL_FIXUP_VALUE_ADDR(value) ((ElfW(Addr)) value)

#undef DL_FIXUP_ADDR_VALUE
#define DL_FIXUP_ADDR_VALUE(addr) ((pfn) addr)

#endif /* DL_LOOKUPCFG_E2K128  */
