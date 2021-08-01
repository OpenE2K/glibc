/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Override generic sotruss-lib.c to define actual functions for E2K.
   Copyright (C) 2012-2017 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#define HAVE_ARCH_PLTENTER
#define HAVE_ARCH_PLTEXIT

#include <elf/sotruss-lib.c>

#if __WORDSIZE == 32
# define la_e2k_gnu_pltenter	la_e2k32_gnu_pltenter
# define la_e2k_gnu_pltexit	la_e2k32_gnu_pltexit
# define La_e2k_regs		La_e2k32_regs
# define La_e2k_retval		La_e2k32_retval
#else /* __WORDSIZE != 32  */
# define la_e2k_gnu_pltenter	la_e2k64_gnu_pltenter
# define la_e2k_gnu_pltexit	la_e2k64_gnu_pltexit
# define La_e2k_regs		La_e2k64_regs
# define La_e2k_retval		La_e2k64_retval
#endif

ElfW(Addr)
la_e2k_gnu_pltenter (ElfW(Sym) *sym __attribute__ ((unused)),
                     unsigned int ndx __attribute__ ((unused)),
                     uintptr_t *refcook,
		     uintptr_t *defcook,
                     La_e2k_regs *regs __attribute__ ((unused)),
		     unsigned int *flags,
                     const char *symname,
		     long int *framesizep)
{
  print_enter (refcook, defcook, symname,
	       /* regs->lr_reg[0], regs->lr_reg[1], regs->lr_reg[2],  */
	       0L, 0L, 0L,
	       *flags);

  /* No need to copy anything, we will not need the parameters in any case.  */
  *framesizep = 0;

  return sym->st_value;
}

unsigned int
la_e2k_gnu_pltexit (ElfW(Sym) *sym __attribute__ ((unused)),
		    unsigned int ndx __attribute__ ((unused)),
		    uintptr_t *refcook,
                    uintptr_t *defcook,
		    const struct La_e2k_regs *inregs __attribute__ ((unused)),
                    struct La_e2k_retval *outregs __attribute__ ((unused)),
		    const char *symname)
{
  print_exit (refcook, defcook, symname, /* outregs->lrv_reg[0]  */ 0L);
  return 0;
}

#undef la_e2k_gnu_pltenter
#undef la_e2k_gnu_pltexit
#undef La_e2k_regs
#undef La_e2k_retval
