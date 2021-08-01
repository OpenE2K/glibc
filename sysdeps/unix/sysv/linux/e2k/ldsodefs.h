/* Run-time dynamic linker data structures for loaded ELF shared objects. E2k.
   Copyright (C) 2001-2013 Free Software Foundation, Inc.
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

#ifndef	_LINUX_E2K_LDSODEFS_H

# define _LINUX_E2K_LDSODEFS_H	1

/* Get the real definitions.  */
#include_next <ldsodefs.h>

#undef VALID_ELF_HEADER
#undef VALID_ELF_OSABI
#undef VALID_ELF_ABIVERSION

/* e_ident[EI_OSABI] byte in the header of legacy EM_E2K_OLD ELFs is used in a
   rather unconvential way to store the so-called "semantics", which is why
   E2K-specific implementations of these macros are needed.  */
#define VALID_ELF_ABIVERSION(osabi, ver)	(ver == 0)

#if !defined VALID_ELF_FLAGS
# define VALID_ELF_FLAGS(flg) (((flg) & EF_E2K_PM) == 0)
#endif

#define VALID_ELF_OSABI(hdr, osabi)					\
  ({									\
    int res;								\
    ElfW(Ehdr) *__hdr = (hdr);						\
    res = ((__hdr->e_machine == EM_E2K_OLD && osabi == E2K_OSABI)	\
	   || (__hdr->e_machine == EM_MCST_ELBRUS			\
	       && (osabi == ELFOSABI_SYSV				\
		   || osabi == ELFOSABI_GNU)				\
	       && VALID_ELF_FLAGS (__hdr->e_flags))			\
	   );								\
    res;								\
  })

#ifndef WITH_BUG_75842
# define VALID_ELF_HEADER(hdr,exp,size)                                 \
  ({                                                                    \
    ElfW(Ehdr) *_hdr = (hdr);                                           \
    (memcmp (_hdr->e_ident, (exp), (size) - 2) == 0                     \
     && VALID_ELF_OSABI (_hdr, _hdr->e_ident[EI_OSABI])			\
     && VALID_ELF_ABIVERSION (_hdr->e_ident[EI_OSABI], _hdr->e_ident[EI_ABIVERSION])); \
      })
#else /* WITH_BUG_75842  */
# define VALID_ELF_HEADER(hdr,exp,size)                                 \
  ({                                                                    \
    ElfW(Ehdr) *_hdr = (hdr);                                           \
    (memcmp (_hdr->e_ident, (exp), (size) - 2) == 0                     \
     && (_hdr->e_flags & EF_E2K_BUG_75842) != 0                         \
     && VALID_ELF_OSABI (_hdr->e_machine, _hdr->e_ident[EI_OSABI])      \
     && VALID_ELF_ABIVERSION (_hdr->e_ident[EI_OSABI], _hdr->e_ident[EI_ABIVERSION]));           \
      })
#endif /* WITH_BUG_75842  */



#endif /* _LINUX_E2K_LDSODEFS  */
