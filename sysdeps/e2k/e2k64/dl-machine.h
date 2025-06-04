#ifndef dl_machine_64_h
#define dl_machine_64_h

#define E2K_OSABI       2

/* The limits in which ICACHE should be flushed after plt header setup. */
#define E2K_RTIME_MIN   8
#define E2K_RTIME_MAX   12

#define E2K_RUNTIME_SETUP                                               \
  do                                                                    \
    {                                                                   \
      Elf64_Addr fixup_addr = (Elf64_Addr) _dl_fixup;                   \
      Elf32_Addr *p_fixup_addr = (Elf32_Addr *) &fixup_addr;            \
      Elf64_Addr l_addr = (Elf64_Addr) l;                               \
      Elf32_Addr *p_l_addr = (Elf32_Addr *) &l_addr;                    \
      Elf64_Addr l_addr_lit =  ((((Elf64_Addr) p_l_addr[0]) << 32)      \
                                + (Elf64_Addr) p_l_addr[1]);            \
                                                                        \
      plt[2] = l_addr_lit;                                              \
      plt[3] = ((((Elf64_Addr) p_fixup_addr[0]) << 32)                  \
                + (Elf64_Addr) p_fixup_addr[1]);                        \
                                                                        \
    } while (0)


#define E2K_RUNTIME_DEFECTIVE_SETUP                                     \
  do                                                                    \
    {                                                                   \
      int i;                                                            \
      int plt_entries = (l->l_info[DT_PLTRELSZ]->d_un.d_val             \
                         / sizeof (Elf64_Rela));                        \
      Elf64_Addr fixup_addr = (Elf64_Addr) _dl_fixup;                   \
      Elf32_Addr *p_fixup_addr = (Elf32_Addr *) &fixup_addr;            \
      Elf64_Addr l_addr = (Elf64_Addr) l;                               \
      Elf32_Addr *p_l_addr = (Elf32_Addr *) &l_addr;                    \
      Elf64_Addr l_addr_lit =  ((((Elf64_Addr) p_l_addr[0]) << 32)      \
                                + (Elf64_Addr) p_l_addr[1]);            \
      Elf64_Addr header_start = (Elf64_Addr) plt;                       \
      Elf32_Addr *p_header_start = (Elf32_Addr *) &header_start;        \
      Elf64_Addr header_start_lit = ((((Elf64_Addr) p_header_start[0]) << 32) \
                                     + (Elf64_Addr) p_header_start[1]); \
                                                                        \
      *plt++ = 0x61c0dcd10c000032ULL;                                   \
      *plt++ = 0x0000000011c0de88ULL;                                   \
      *plt++ = l_addr_lit;                                              \
      *plt++ = ((((Elf64_Addr) p_fixup_addr[0]) << 32)                  \
                + (Elf64_Addr) p_fixup_addr[1]);                        \
      *plt++ = 0x8000042000009012ULL;                                   \
      *plt++ = 0x0000000050000004ULL;                                   \
      *plt++ = 0x61c088d104000001ULL;                                   \
      *plt++ = 0x8000042000001001ULL;                                   \
                                                                        \
      /* Clean up excessive space inside header.  */                    \
      for (i = 0; i < 8; i++)                                           \
        *plt++ = 0x0ULL;                                                \
                                                                        \
      for (i = 0; i < plt_entries; i++)                                 \
        {                                                               \
          Elf32_Addr *entry_start = (Elf32_Addr *) plt;                 \
          Elf32_Addr reloc_offset_lo = entry_start[6];                  \
          Elf32_Addr reloc_offset_hi = entry_start[5];                  \
                                                                        \
          /* The first eight bytes in my old defective PLT entry        \
             are different from the ones in the new entry. Employ       \
             this to find out whether this and all consequent entries   \
             have already been converted. Double conversion is not      \
             harmless because of the need to fetch `reloc_offset'.  */  \
          if (plt[0] != 0x11c0dd880400c233ULL)                          \
            break;                                                      \
                                                                        \
          *plt++ = 0x61c0dcd104000011ULL;                               \
          *plt++ = header_start_lit;                                    \
          *plt++ = 0x8000042004009033ULL;                               \
          *plt++ = 0x0000000011c0dd89ULL;                               \
          *plt++ = (((Elf64_Addr) reloc_offset_hi) << 32);              \
          *plt++ = ((0x00000110ULL << 32)                               \
                    + (Elf64_Addr) reloc_offset_lo);                    \
          *plt++ = 0x0ULL;                                              \
          *plt++ = 0x0ULL;                                              \
        }                                                               \
    } while (0)

#define E2K_RUNTIME_BUG_75842_SETUP                                     \
  do                                                                    \
    {                                                                   \
      int i;                                                            \
      int plt_entries = (l->l_info[DT_PLTRELSZ]->d_un.d_val             \
                         / sizeof (Elf64_Rela));                        \
      Elf64_Addr fixup_addr = (Elf64_Addr) _dl_fixup;                   \
      Elf32_Addr *p_fixup_addr = (Elf32_Addr *) &fixup_addr;            \
      Elf64_Addr l_addr = (Elf64_Addr) l;                               \
      Elf32_Addr *p_l_addr = (Elf32_Addr *) &l_addr;                    \
      Elf64_Addr l_addr_lit =  ((((Elf64_Addr) p_l_addr[0]) << 32)      \
                                + (Elf64_Addr) p_l_addr[1]);            \
      Elf64_Addr header_start = (Elf64_Addr) plt;                       \
      Elf32_Addr *p_header_start = (Elf32_Addr *) &header_start;        \
      Elf64_Addr header_start_lit = ((((Elf64_Addr) p_header_start[0]) << 32) \
                                     + (Elf64_Addr) p_header_start[1]); \
                                                                        \
      plt[2] = l_addr_lit;                                              \
      plt[3] = ((((Elf64_Addr) p_fixup_addr[0]) << 32)                  \
                + (Elf64_Addr) p_fixup_addr[1]);                        \
      /* Step over the PLT header.  */                                  \
      plt += 24;                                                        \
                                                                        \
      for (i = 0; i < plt_entries; i++)                                 \
        {                                                               \
          plt[20] = header_start_lit;                                   \
          plt += 24;                                                    \
        }                                                               \
    } while (0)

#define E2K_FIXUP_PLT(reloc_addr, val)                                  \
  do                                                                    \
    {                                                                   \
      *((Elf32_Addr *) reloc_addr) = (Elf32_Addr) (val >> 32);          \
      *((Elf32_Addr *) reloc_addr + 1)                                  \
        = (Elf32_Addr) (val & 0xffffffff);                              \
    } while (0)  


#define E2K_FIXUP_LAZY_BUG_75842_PLT(reloc_addr, val)                   \
  do                                                                    \
    {                                                                   \
      Elf64_Addr *reloc_addr64 = (Elf64_Addr *) reloc_addr;             \
      Elf64_Addr val64 = val;                                           \
      Elf32_Addr *p_val64 = (Elf32_Addr *) &val64;                      \
      Elf64_Addr lit_val = ((((Elf64_Addr) p_val64[0]) << 32)           \
                            + (Elf64_Addr) p_val64[1]);                 \
      reloc_addr64[20] = lit_val;                                       \
      flush_icache_range ((unsigned long) &reloc_addr64[20],            \
                          (unsigned long) &reloc_addr64[21]);           \
    } while (0)

#define PROCESS_R_E2K_ABS(reloc_addr, val)                              \
  case R_E2K_32_ABS:							\
  /* Recognize "empty" dynamic relocation entries filled in with	\
     zeroes. They used to be emitted by an outdated e2k-linux-ld when	\
     more space was allocated for dynamic relocations than needed.	\
     Modern e2k-linux-ld should emit `R_E2K_NONE != 0's in such a case. \
     TODO: this check should be eventually removed.  */			\
 if (_reloc->r_offset)							\
      *_reloc_addr = value;                                             \
 break;                                                                 \
  case R_E2K_64_ABS:                                                    \
    *reloc_addr = val;                                                  \
    break;                                                              \
 case R_E2K_64_ABS_LIT:                                                 \
   *((Elf32_Addr *) reloc_addr) = (Elf32_Addr) (value >> 32);           \
   *((Elf32_Addr *) reloc_addr + 1) = (Elf32_Addr) (value & 0xffffffff);\
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
            "  movtd,0 %dr8, %ctpr1\n"                                  \
            "  ldd,2 %dr0, [$_dl_fini@GOT], %dr8\n"                     \
            "}\n"                                                       \
            "call %ctpr1, wbs = 0x4\n"                                  \
            "stw,2 0x0, 0x0, %r1\n" );

#endif /* dl_machine_64_h */
