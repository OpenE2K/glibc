#ifndef dl_machine_32_h
#define dl_machine_32_h

#define E2K_OSABI       1

/* The limits in which ICACHE should be flushed after plt header setup. */
#define E2K_RTIME_MIN   4
#define E2K_RTIME_MAX   10


#define E2K_RUNTIME_SETUP                                               \
  do                                                                    \
    {                                                                   \
      Elf32_Addr fixup_addr = (Elf32_Addr) _dl_fixup;                   \
      Elf32_Addr l_addr = (Elf32_Addr) l;                               \
                                                                        \
      plt[4] = l_addr;                                                  \
      plt[5] = fixup_addr;                                              \
    } while (0)

#define E2K_RUNTIME_DEFECTIVE_SETUP                                     \
  do                                                                    \
    {                                                                   \
      int i;                                                            \
      int plt_entries = (l->l_info[DT_PLTRELSZ]->d_un.d_val             \
                         / sizeof (Elf32_Rela));                        \
      Elf32_Addr fixup_addr = (Elf32_Addr) _dl_fixup;                   \
      Elf32_Addr l_addr = (Elf32_Addr) l;                               \
      Elf32_Addr header_start = (Elf32_Addr) plt;                       \
                                                                        \
      *plt++ = 0x0c000022;                                              \
      *plt++ = 0x63f0d8d1;                                              \
      *plt++ = 0x10c0d988;                                              \
      *plt++ = 0x00000000;                                              \
      *plt++ = l_addr;                                                  \
      *plt++ = fixup_addr;                                              \
                                                                        \
      *plt++ = 0x00009012;                                              \
      *plt++ = 0x80000420;                                              \
      *plt++ = 0x50000004;                                              \
      *plt++ = 0x00000000;                                              \
                                                                        \
      *plt++ = 0x04000001;                                              \
      *plt++ = 0x63f088d1;                                              \
                                                                        \
      *plt++ = 0x00001001;                                              \
      *plt++ = 0x80000420;                                              \
                                                                        \
      /* Clean up excessive space inside header. 14 of 32 words per     \
         header have been used.  */                                     \
      for (i = 0; i < 18; i++)                                          \
        *plt++ = 0x0ULL;                                                \
                                                                        \
      for (i = 0; i < plt_entries; i++)                                 \
        {                                                               \
          Elf32_Addr *entry_start = plt;                                \
          Elf32_Addr reloc_offset = entry_start[7];                     \
                                                                        \
          /* The first eight bytes in my old defective PLT entry        \
             are different from the ones in the new entry. Employ       \
             this to find out whether this and all consequent entries   \
             have already been converted. Double conversion is not      \
             harmless because of the need to fetch `reloc_offset'.  */  \
          if (plt[0] != 0x0000c012)                                     \
            break;                                                      \
                                                                        \
          *plt++ = 0x04000011;                                          \
          *plt++ = 0x63f0d8d1;                                          \
          *plt++ = 0x00000000;                                          \
          *plt++ = header_start;                                        \
                                                                        \
          *plt++ = 0x04009023;                                          \
          *plt++ = 0x80000420;                                          \
          *plt++ = 0x10c0d989;                                          \
          *plt++ = 0x00000000;                                          \
          *plt++ = reloc_offset;                                        \
          *plt++ = 0x00000110;                                          \
                                                                        \
          /* Cleaning the remaining 6 of 16 words per entry.  */        \
                                                                        \
          *plt++ = 0x0ULL;                                              \
          *plt++ = 0x0ULL;                                              \
          *plt++ = 0x0ULL;                                              \
          *plt++ = 0x0ULL;                                              \
          *plt++ = 0x0ULL;                                              \
          *plt++ = 0x0ULL;                                              \
        }                                                               \
    } while (0)

#define E2K_RUNTIME_BUG_75842_SETUP                                     \
  do                                                                    \
    {                                                                   \
      int i;                                                            \
      int plt_entries = (l->l_info[DT_PLTRELSZ]->d_un.d_val             \
                         / sizeof (Elf32_Rela));                        \
      Elf32_Addr fixup_addr = (Elf32_Addr) _dl_fixup;                   \
      Elf32_Addr l_addr = (Elf32_Addr) l;                               \
      Elf32_Addr header_start = (Elf32_Addr) plt;			\
                                                                        \
      plt[4] = l_addr;                                                  \
      plt[5] = fixup_addr;                                              \
                                                                        \
      /* Step over the PLT header.  */                                  \
      plt += 46;                                                        \
                                                                        \
      for (i = 0; i < plt_entries; i++)                                 \
        {                                                               \
          plt[41] = header_start;                                       \
          plt += 46;                                                    \
        }                                                               \
    } while (0)

#define E2K_FIXUP_PLT(reloc_addr, val)          \
  do                                            \
    {                                           \
      *reloc_addr = val;                        \
    } while (0)


#define E2K_FIXUP_LAZY_BUG_75842_PLT(reloc_addr, val)           \
  do                                                            \
    {                                                           \
      reloc_addr[41] = val;                                     \
      flush_icache_range ((unsigned long ) &reloc_addr[41],     \
                          (unsigned long) &reloc_addr[42]);     \
    } while (0)

#define PROCESS_R_E2K_ABS(reloc_addr, val)                              \
  case R_E2K_32_ABS:                                                    \
  /* Recognize "empty" dynamic relocation entries filled in with	\
     zeroes. They used to be emitted by an outdated e2k-linux-ld when	\
     more space was allocated for dynamic relocations than needed.	\
     Modern e2k-linux-ld should emit `R_E2K_NONE != 0's in such a case. \
     TODO: this check should be eventually removed.  */			\
if (_reloc->r_offset)							\
      *_reloc_addr = value;                                             \
  break;


#define RTLD_START_EARLY                                                \
  __asm__ ( "\n"                                                        \
            ".text\n"                                                   \
            ".global $_start\n"                                         \
            ".type $_start, @function\n"                                \
            ".align 32\n"                                               \
            "$_start:\n"                                                \
            "{\n"                                                       \
            "  setwd wsz = 0x8, nfx = 0x0\n"                            \
            "  rrd,0 %ip, %dr0\n"                                       \
            "  addd,1 0x0, [_lts1 $_GLOBAL_OFFSET_TABLE_], %dr1\n"      \
            "  disp %ctpr1, $_start2\n"                                 \
            "}\n"                                                       \
            "{\n"                                                       \
            "  call %ctpr1, wbs = 0x4\n"                                \
            "  addd,0 %dr0, %dr1, %dr0\n"                               \
            "  adds,1 0x0, 0x0, %r1\n"                                  \
            "}\n"                                                       \
            "{\n"                                                       \
            "  getpl,0 %r8, %ctpr1\n"                                   \
            "  ldgdw,2 %r0, [$_dl_fini@GOT], %r8\n"                     \
            "}\n"                                                       \
            "call %ctpr1, wbs = 0x4\n"                                  \
            "stgdw,2 0x0, 0x0, %r1\n" );

#endif /* dl_machine_32_h */
