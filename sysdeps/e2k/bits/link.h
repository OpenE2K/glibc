/* Machine-specific audit interfaces for dynamic linker.  E2K version.
   Copyright (C) 2008 Free Software Foundation, Inc.
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

#ifndef	_LINK_H
# error "Never include <bits/link.h> directly; use <link.h> instead."
#endif

#if __WORDSIZE == 32

/* Registers for entry into PLT on e2k32.  */
typedef struct La_e2k32_regs
{
  uint32_t lr_r8;
  uint32_t lr_r9;
  uint32_t lr_r10;
  uint32_t lr_r11;
  uint32_t lr_gr [8];
  long double lr_fr [8];
  uint32_t lr_unat;
  uint32_t lr_sp;
} La_e2k32_regs;

/* Return values for calls from PLT on e2k32.  */
typedef struct La_e2k32_retval
{
  uint32_t lrv_r8;
  uint32_t lrv_r9;
  uint32_t lrv_r10;
  uint32_t lrv_r11;
  long double lr_fr [8];
} La_e2k32_retval;

#else

/* Registers for entry into PLT on e2k64.  */
typedef struct La_e2k64_regs
{
  uint64_t lr_r8;
  uint64_t lr_r9;
  uint64_t lr_r10;
  uint64_t lr_r11;
  uint64_t lr_gr [8];
  long double lr_fr [8];
  uint64_t lr_unat;
  uint64_t lr_sp;
} La_e2k64_regs;

/* Return values for calls from PLT on e2k64.  */
typedef struct La_e2k64_retval
{
  uint64_t lrv_r8;
  uint64_t lrv_r9;
  uint64_t lrv_r10;
  uint64_t lrv_r11;
  long double lr_fr [8];
} La_e2k64_retval;

#endif

__BEGIN_DECLS

#if __WORDSIZE == 32

extern ElfW(Addr) la_e2k32_gnu_pltenter (ElfW(Sym) *__sym,
					 unsigned int __ndx,
					 uintptr_t *__refcook,
					 uintptr_t *__defcook,
					 La_e2k32_regs *__regs,
					 unsigned int *__flags,
					 const char *__symname,
					 long int *__framesizep);
extern unsigned int la_e2k32_gnu_pltexit (ElfW(Sym) *__sym,
					  unsigned int __ndx,
					  uintptr_t *__refcook,
					  uintptr_t *__defcook,
					  const La_e2k32_regs *__inregs,
					  La_e2k32_retval *__outregs,
					  const char *symname);

#else

extern ElfW(Addr) la_e2k64_gnu_pltenter (ElfW(Sym) *__sym,
					 unsigned int __ndx,
					 uintptr_t *__refcook,
					 uintptr_t *__defcook,
					 La_e2k64_regs *__regs,
					 unsigned int *__flags,
					 const char *__symname,
					 long int *__framesizep);
extern unsigned int la_e2k64_gnu_pltexit (ElfW(Sym) *__sym,
					  unsigned int __ndx,
					  uintptr_t *__refcook,
					  uintptr_t *__defcook,
					  const La_e2k64_regs *__inregs,
					  La_e2k64_retval *__outregs,
					  const char *symname);

#endif

__END_DECLS
