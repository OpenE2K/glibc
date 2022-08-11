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
