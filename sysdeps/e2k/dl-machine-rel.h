#ifndef _DL_MACHINE_REL_H
#define _DL_MACHINE_REL_H

#define ELF_MACHINE_NO_REL      1
#define ELF_MACHINE_NO_RELA     0

/* Used to calculate the index of link_map l_reloc_result.  */
#define PLTREL ElfW(Rela)

#endif
