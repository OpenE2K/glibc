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

#ifndef R_E2K_128_IRELATIVE

#define ELF_MACHINE_IRELA	1

static __attribute__ ((unused)) pfn
elf_ifunc_invoke (pfn addr)
{
  return ((pfn (*) (void)) addr) ();
}


static __attribute__ ((unused)) void
elf_irela (const ElfW(Rela) *reloc)
{
}

#endif /* R_E2K_128_IRELATIVE  */
