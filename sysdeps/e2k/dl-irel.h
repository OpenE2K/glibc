/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Machine-dependent ELF indirect relocation functions.
   E2K version.
   Copyright (C) 2016 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#ifndef _DL_IREL_H
#define _DL_IREL_H

#include <stdio.h>

#define ELF_MACHINE_IRELA	1

static ElfW(Addr)
elf_ifunc_invoke (ElfW(Addr) addr)
{
  return ((ElfW(Addr) (*) (void)) (addr)) ();
}

static __attribute__ ((unused)) void
elf_irela (const ElfW(Rela) *reloc)
{
  ElfW(Addr) *const reloc_addr = (void *) reloc->r_offset;
  const unsigned long int r_type = ELFW(R_TYPE) (reloc->r_info);

  if (__glibc_likely (r_type == R_E2K_XX_IRELATIVE))
    {
      ElfW(Addr) value = elf_ifunc_invoke(reloc->r_addend);
      *reloc_addr = value;
    }
  else
    __libc_fatal ("unexpected reloc type in static binary");
}

#endif /* dl-irel.h */
