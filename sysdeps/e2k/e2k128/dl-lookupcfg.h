/* Copyright (c) 2009-2024 AO MCST.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

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

#undef DL_FIXUP_BINDNOW_RELOC
#define DL_FIXUP_BINDNOW_RELOC(map, value, new_value, st_value)	\
  (*value) = DL_FIXUP_MAKE_VALUE (map, st_value);

#endif /* DL_LOOKUPCFG_E2K128  */
