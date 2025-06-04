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
#undef MORE_ELF_HEADER_DATA

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

/* When non-NULL ERRMSG points to char[] buffer filled in with error
   message.  */
#define VALID_ELF_MACHINE(hdr, errmsg)					\
  ({                                                                    \
  int res;                                                              \
  ElfW(Ehdr) *__hdr = (hdr);                                            \
  res = runtime_compatible_host (__hdr->e_flags, errmsg);		\
  res;                                                                  \
})

#ifndef WITH_BUG_75842
# define VALID_ELF_HEADER(hdr,exp,size)                                 \
  ({                                                                    \
    ElfW(Ehdr) *_hdr = (hdr);                                           \
    (memcmp (_hdr->e_ident, (exp), (size) - 2) == 0                     \
     && VALID_ELF_OSABI (_hdr, _hdr->e_ident[EI_OSABI])			\
     && VALID_ELF_MACHINE (_hdr, NULL)					\
     && VALID_ELF_ABIVERSION (_hdr->e_ident[EI_OSABI], _hdr->e_ident[EI_ABIVERSION])); \
      })
#else /* WITH_BUG_75842  */
# define VALID_ELF_HEADER(hdr,exp,size)                                 \
  ({                                                                    \
    ElfW(Ehdr) *_hdr = (hdr);                                           \
    (memcmp (_hdr->e_ident, (exp), (size) - 2) == 0                     \
     && (_hdr->e_flags & EF_E2K_BUG_75842) != 0                         \
     && VALID_ELF_OSABI (_hdr->e_machine, _hdr->e_ident[EI_OSABI])      \
     && VALID_ELF_MACHINE (_hdr, NULL)					\
     && VALID_ELF_ABIVERSION (_hdr->e_ident[EI_OSABI], _hdr->e_ident[EI_ABIVERSION]));           \
      })
#endif /* WITH_BUG_75842  */


/* The implementation is inspired by elf_check_e2k_mtype ()
   in the Linux Kernel.  */
static inline int
runtime_compatible_host (uint32_t flg, char *errmsg)
{
  const uint32_t elf_mach = (flg >> 24) & 0xff;
  int incompat = ((flg & EF_E2K_INCOMPAT) != 0);
  /* "Regular" elbrus-v{X>=7} ELFs should not be executed on deficient
     elbrus-maket32c (see Bug #161286, Comment #7 and a slight
     refinement in Bug #159001, Comment #26).  */
  const int maket =
#ifdef __MCST_SUPPORT_ELBRUS_MAKET32C__
    __builtin_cpu_is ("elbrus-maket32c")
#else /* ! defined __MCST_SUPPORT_ELBRUS_MAKET32C__  */
    0
#endif /* ! defined __MCST_SUPPORT_ELBRUS_MAKET32C__  */
    ;

  int iset;
  /* __builtin_e2k_cpu_iset () could be more naturally used for this purpose
     starting from rel-29-0 (Bug #162253).  */
  if (__builtin_cpu_is ("elbrus-v7"))
    iset = 7;
  else if (__builtin_cpu_is ("elbrus-v6"))
    iset = 6;
  else if (__builtin_cpu_is ("elbrus-v5"))
    iset = 5;
  else if (__builtin_cpu_is ("elbrus-v4"))
    iset = 4;
  else if (__builtin_cpu_is ("elbrus-v3"))
    iset = 3;
  else
    /* Processors of no other isets currently exist now that
       elbrus-v1 has been dead for more than a decade.  */
    iset = 2;

  switch (elf_mach)
    {
    case 2:
      if (!maket && ((!incompat && iset > 2) || iset == 2))
	return 1;
      break;
    case 3:
      if (!maket && ((!incompat && iset > 3) || iset == 3))
	return 1;
      break;
    case 4:
      if (!maket && ((!incompat && iset > 4) || iset == 4))
	return 1;
      break;
    case 5:
      if (!maket && ((!incompat && iset > 5) || iset == 5))
	return 1;
      break;
    case 6:
      if (!maket && ((!incompat && iset > 6) || iset == 6))
	return 1;
      break;
    case 7:
      if (!maket && ((! incompat && iset > 7) || iset == 7))
	return 1;
      break;
    case E_E2K_MACH_8C:
      if (__builtin_cpu_is ("elbrus-8c"))
	return 1;
      break;
    case E_E2K_MACH_1CPLUS:
      if (__builtin_cpu_is ("elbrus-1c+"))
	return 1;
      break;
    case E_E2K_MACH_12C:
      if (__builtin_cpu_is ("elbrus-12c"))
	return 1;
      break;
    case E_E2K_MACH_16C:
      if (__builtin_cpu_is ("elbrus-16c"))
	return 1;
      break;
    case E_E2K_MACH_2C3:
      if (__builtin_cpu_is ("elbrus-2c3"))
	return 1;
      break;
    case E_E2K_MACH_48C:
      if (__builtin_cpu_is ("elbrus-48c"))
	return 1;
      break;
    case E_E2K_MACH_8V7:
      if (__builtin_cpu_is ("elbrus-8v7"))
	return 1;
      break;
#ifdef __MCST_SUPPORT_ELBRUS_MAKET32C__
    case E_E2K_MACH_MAKET32C:
      /* elbrus-maket32c ELFs should be allowed to run on elbrus-maket32c
	 only.  */
      if (maket)
	return 1;
#endif /* __MCST_SUPPORT_ELBRUS_MAKET32C__  */
    }

  if (errmsg != NULL)
    {
      int i, j = 0;
      const char *incompat_machine_1 = "ELF file for ";
      for (i = 0; incompat_machine_1[i] != '\0'; i++)
	errmsg[j++] = incompat_machine_1[i];

      static const char *models[] =
	{"unknown",
	 "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	 "8c", "1c+",
	 "12c", "16c", "2c3",
	 "48c", "8v7", "maket32c"};

      uint32_t idx = elf_mach;
      if (idx >= 19 && idx <= 26)
	idx = 8 + (idx - 19);
      else if (idx > 7)
	idx = 0;

      const char *model = models[idx];

      if (incompat)
	{
	  /* No point if a special name is available.  */
	  incompat = 0;

	  if (idx == 2)
	    model = "2c+";
	  else if (idx == 3)
	    model = "4c";
	  else if (idx == 5)
	    model = "8c2";
	  else
	    /* No special name. Restore "incompat".  */
	    incompat = 1;
	}

      if (incompat)
	{
	  const char *forward_incompat = "forward incompatible ";
	  for (i = 0; forward_incompat[i] != '\0'; i++)
	    errmsg[j++] = forward_incompat[i];
	}

      const char *elbrus_pfx = "elbrus-";
      for (i = 0; elbrus_pfx[i] != '\0'; i++)
	errmsg[j++] = elbrus_pfx[i];

      for (i = 0; model[i] != '\0'; i++)
	errmsg[j++] = model[i];

      const char *incompat_machine_2 = " is incompatible with your elbrus-";
      for (i = 0; incompat_machine_2[i] != '\0'; i++)
	errmsg[j++] = incompat_machine_2[i];

      const char *host = "unknown";
      if (__builtin_cpu_is ("elbrus-2c+"))
	host = "2c+";
      else if (__builtin_cpu_is ("elbrus-4c"))
	host = "4c";
      else if (__builtin_cpu_is ("elbrus-8c"))
	host = "8c";
      else if (__builtin_cpu_is ("elbrus-1c+"))
	host = "1c+";
      else if (__builtin_cpu_is ("elbrus-8c2"))
	host = "8c2";
      else if (__builtin_cpu_is ("elbrus-12c"))
	host = "12c";
      else if (__builtin_cpu_is ("elbrus-16c"))
	host = "16c";
      else if (__builtin_cpu_is ("elbrus-2c3"))
	host = "2c3";
      else if (__builtin_cpu_is ("elbrus-48c"))
	host = "48c";
      else if (__builtin_cpu_is ("elbrus-8v7"))
	host = "8v7";
#ifdef __MCST_SUPPORT_ELBRUS_MAKET32C__
      else if (__builtin_cpu_is ("elbrus-maket32c"))
	host = "maket32c";
#endif /* __MCST_SUPPORT_ELBRUS_MAKET32C__  */

      for (i = 0; host[i] != '\0'; i++)
	errmsg[j++] = host[i];

      const char *incompat_machine_3 = " host";
      for (i = 0; incompat_machine_3[i] != '\0'; i++)
	errmsg[j++] = incompat_machine_3[i];
    }

  return 0;
}

#endif /* _LINUX_E2K_LDSODEFS  */
