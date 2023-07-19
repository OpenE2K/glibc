/* Map in a shared object's segments.  Generic version.
   Copyright (C) 1995-2019 Free Software Foundation, Inc.
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

#include <dl-load.h>

/* This implementation assumes (as does the corresponding implementation
   of _dl_unmap_segments, in dl-unmap-segments.h) that shared objects
   are always laid out with all segments contiguous (or with gaps
   between them small enough that it's preferable to reserve all whole
   pages inside the gaps with PROT_NONE mappings rather than permitting
   other use of those parts of the address space).  */

static __always_inline const char *
_dl_map_segments (struct link_map *l, int fd,
                  const ElfW(Ehdr) *header, int type,
                  const struct loadcmd loadcmds[], size_t nloadcmds,
                  const size_t maplength, bool has_holes,
                  struct link_map *loader
#if defined __ptr128__
		  , const ElfW(Phdr) *phdr,
		  int *perrval
#endif /* defined __ptr128__  */
		  )
{
#if defined __ptr128__
  mdd_t mdd;
  int residue = fd;
  char fdstr[10];
  char buf[14 + sizeof (fdstr) + 1] = "/proc/self/fd/";
  int i, cnt = 0;
  const char *name;
  const struct loadcmd *c = loadcmds;

  do
    {
      char digit = residue % 10;
      fdstr[cnt++] = '0' + digit;
      residue = (residue - digit) / 10;
    }
  while (residue != 0 && cnt < sizeof (fdstr));

  /* This should  be impossible on practice. Just return the most general error
     code if this manages to happen somehow. */
  if (residue != 0)
    return DL_MAP_SEGMENTS_ERROR_MAP_SEGMENT;

  for (i = 0; i < cnt; i++)
    buf[14 + i] = fdstr[cnt - 1 - i];

  buf[14 + i] = '\0';
  name = buf;

  if (INLINE_SYSCALL_CALL (newuselib, name, &mdd) != 0)
    /* This one of the standard DL_MAP_SEGMENTS_ERROR_* codes seems to be the
       most relevant to the situation, though not quite perfect.  */
    return DL_MAP_SEGMENTS_ERROR_MAP_SEGMENT;

  /* Let it initialize itself as soon as possible.  */
  mdd.selfinit ();

  l->l_gd = mdd.gd;

  if (! (header->e_flags & EF_E2K_PACK_SEGMENTS))
    {
      /* If l_phdr still contains "NULL" there's no PT_PHDR in its ELF as
	 it takes place in libm.so. In such a case there's no other way of
	 getting access to Program Headers in legacy PM case (as well as in
	 ordinary modes) except for `malloc ()'ing and memcpy ()'ing them
	 from PHDR (the temporary  buffer they were read to above) even
	 though  they are almost sure to be present in GD (see the
	 evaluation in the packed case relying on the latter).  */
      if (l->l_phdr != NULL)
	l->l_phdr = l->l_gd + (long) l->l_phdr;
      else
	{
	  /* FIXME(?): this is an ugly copy/paste of the analogous code for
	     ordinary modes in `elf/dl-load.c'. See if the evaluation of
	     "runtime" l_phdr value at an earlier stage (i.e. here) at which
	     it's needed in PM is going to break anything in ordinary modes. */

	  /* The program header is not contained in any of the segments.
	     We have to allocate memory ourselves and copy it over from out
	     temporary place.  */
	  ElfW(Phdr) *newp = (ElfW(Phdr) *) malloc (header->e_phnum
						    * sizeof (ElfW(Phdr)));
	  if (newp == NULL)
	    {
	      /* `errval = errno; goto lose;' in elf/dl-load.c is the same as
		 `goto lose_errno;' in case of an analogous `malloc ()' failure
		 in ordinary modes.  */
	      *perrval = errno;
	      return N_("cannot allocate memory for program header");
	    }

	  l->l_phdr = memcpy (newp, phdr,
			      (header->e_phnum * sizeof (ElfW(Phdr))));
	  l->l_phdr_allocated = 1;
	}
    }
  else
    /* In packed case rely on the assumption that the containing segment
       is mapped starting from 0 offset in GD. Note that get_offset ()
       can't be used before l_phdr is set.  */
    l->l_phdr = l->l_gd + header->e_phoff;


  /* FIXME: I thought of explicitly setting `l_addr' to zero since it should
     play no role in PM, but it turns out to be that for the main executable's
     link_map it's likely to be re-set in `dl_main ()' after we have returned
     from this function. (I guess this is done because there's no other way to
     obtain `l_addr' in ordinary modes if the main executable has been loaded
     by the Kernel, which happens if ld.so is started implicitly or, in other
     words, as an interpreter. Shouldn't I eventually eliminate that code for
     PM?) Therefore, for symmetry with other `link_map's set it here to its
     actual value which is the data segment base address. Note that it may be
     required for (PM-specific) interface with GDB.  */
  l->l_addr = (ElfW(Addr)) l->l_gd;
  l->l_code_addr = (unsigned long) mdd.selfinit;

  /* `l_map_start' and `l_text_end' are used in `_dl_check_caller ()' (see
     `__check_caller (args->caller_dl_open, . . .)') to check if the `_dl_open
     ()' caller's %pc belongs to one of the allowed libraries. Set them more or
     less by analogy with ordinary modes below. FIXME: consider implementing
     this code more symmetrically to the underlying one and in particular the
     use of `_dl_postprocess_loadcmd ()' responsible for setting `l_text_end'
     in non-Protected modes.  */
  l->l_text_start = (unsigned int) -1;
  l->l_text_end = 0;
  l->l_data_start = (unsigned int) -1;
  l->l_data_end = 0;
  for (c = loadcmds; c < &loadcmds[nloadcmds]; ++c)
    {
      if (c->prot & PROT_EXEC)
	{
	  if (l->l_text_start > get_offset_ex (l, c->mapstart, 1) + l->l_code_addr)
	    l->l_text_start = get_offset_ex (l, c->mapstart, 1) + l->l_code_addr;

	  /* This matches the way they set `l_text_end' in `_dl_postprocess_
	     loadcmd ()': the last page mapped on file gives contribution to
	     `[l_text_start; l_text_end)', while the `.bss'-like portion of the
	     last executable Program Header is excluded.  */
	  if (l->l_text_end < get_offset_ex (l, c->mapend, 1) + l->l_code_addr)
	    l->l_text_end = get_offset_ex (l, c->mapend, 1) + l->l_code_addr;
	}
      else
	{
	  if (l->l_data_start > get_offset (l, c->mapstart) + l->l_addr)
	    l->l_data_start = get_offset (l, c->mapstart) + l->l_addr;

	  /* I'm incapable of setting this value by analogy with l_map_end in
	     ordinary modes below because I make no use of maplength parameter.
	     Moreover, this probably makes little sense. However, I believe that
	     the `.bss' portion of the last data Program Header SHOULD be taken
	     into account here unlike l_text_end above.  */
	  if (l->l_data_end < (ALIGN_UP (get_offset (l, c->allocend), GLRO (dl_pagesize))
			       + l->l_addr))
	    l->l_data_end = (ALIGN_UP (get_offset (l, c->allocend), GLRO (dl_pagesize))
			     + l->l_addr);
	}
    }

  /* If we are unable to get any info either on code or data Program Headers
     believe that the corresponding segments are of zero length. This should
     prevent us from associating any address or symbol with them.  */
     
  if (l->l_text_start == (unsigned int) -1)
    l->l_text_start = l->l_text_end = l->l_code_addr;

  if (l->l_data_start == (unsigned int) -1)
    l->l_data_start = l->l_data_end = l->l_addr;

  return NULL;

#else /* ! defined __ptr128__  */

  const struct loadcmd *c = loadcmds;

  if (__glibc_likely (type == ET_DYN))
    {
      /* This is a position-independent shared object.  We can let the
         kernel map it anywhere it likes, but we must have space for all
         the segments in their specified positions relative to the first.
         So we map the first segment without MAP_FIXED, but with its
         extent increased to cover all the segments.  Then we remove
         access from excess portion, and there is known sufficient space
         there to remap from the later segments.

         As a refinement, sometimes we have an address that we would
         prefer to map such objects at; but this is only a preference,
         the OS can do whatever it likes. */
      ElfW(Addr) mappref
        = (ELF_PREFERRED_ADDRESS (loader, maplength,
                                  c->mapstart & GLRO(dl_use_load_bias))
           - MAP_BASE_ADDR (l));

      /* Remember which part of the address space this object uses.  */
      l->l_map_start = (ElfW(Addr)) __mmap ((void *) mappref, maplength,
                                            c->prot,
                                            MAP_COPY|MAP_FILE,
                                            fd, c->mapoff);
      if (__glibc_unlikely ((void *) l->l_map_start == MAP_FAILED))
        return DL_MAP_SEGMENTS_ERROR_MAP_SEGMENT;

      l->l_map_end = l->l_map_start + maplength;
      l->l_addr = l->l_map_start - c->mapstart;

      if (has_holes)
        {
          /* Change protection on the excess portion to disallow all access;
             the portions we do not remap later will be inaccessible as if
             unallocated.  Then jump into the normal segment-mapping loop to
             handle the portion of the segment past the end of the file
             mapping.  */
          if (__glibc_unlikely
              (__mprotect ((caddr_t) (l->l_addr + c->mapend),
                           loadcmds[nloadcmds - 1].mapstart - c->mapend,
                           PROT_NONE) < 0))
            return DL_MAP_SEGMENTS_ERROR_MPROTECT;
        }

      l->l_contiguous = 1;

      goto postmap;
    }

  /* Remember which part of the address space this object uses.  */
  l->l_map_start = c->mapstart + l->l_addr;
  l->l_map_end = l->l_map_start + maplength;
  l->l_contiguous = !has_holes;

  while (c < &loadcmds[nloadcmds])
    {
      if (c->mapend > c->mapstart
          /* Map the segment contents from the file.  */
          && (__mmap ((void *) (l->l_addr + c->mapstart),
                      c->mapend - c->mapstart, c->prot,
                      MAP_FIXED|MAP_COPY|MAP_FILE,
                      fd, c->mapoff)
              == MAP_FAILED))
        return DL_MAP_SEGMENTS_ERROR_MAP_SEGMENT;

    postmap:
      _dl_postprocess_loadcmd (l, header, c);

      if (c->allocend > c->dataend)
        {
          /* Extra zero pages should appear at the end of this segment,
             after the data mapped from the file.   */
          ElfW(Addr) zero, zeroend, zeropage;

          zero = l->l_addr + c->dataend;
          zeroend = l->l_addr + c->allocend;
          zeropage = ((zero + GLRO(dl_pagesize) - 1)
                      & ~(GLRO(dl_pagesize) - 1));

          if (zeroend < zeropage)
            /* All the extra data is in the last page of the segment.
               We can just zero it.  */
            zeropage = zeroend;

          if (zeropage > zero)
            {
              /* Zero the final part of the last page of the segment.  */
              if (__glibc_unlikely ((c->prot & PROT_WRITE) == 0))
                {
                  /* Dag nab it.  */
                  if (__mprotect ((caddr_t) (zero
                                             & ~(GLRO(dl_pagesize) - 1)),
                                  GLRO(dl_pagesize), c->prot|PROT_WRITE) < 0)
                    return DL_MAP_SEGMENTS_ERROR_MPROTECT;
                }
              memset ((void *) zero, '\0', zeropage - zero);
              if (__glibc_unlikely ((c->prot & PROT_WRITE) == 0))
                __mprotect ((caddr_t) (zero & ~(GLRO(dl_pagesize) - 1)),
                            GLRO(dl_pagesize), c->prot);
            }

          if (zeroend > zeropage)
            {
              /* Map the remaining zero pages in from the zero fill FD.  */
              caddr_t mapat;
              mapat = __mmap ((caddr_t) zeropage, zeroend - zeropage,
                              c->prot, MAP_ANON|MAP_PRIVATE|MAP_FIXED,
                              -1, 0);
              if (__glibc_unlikely (mapat == MAP_FAILED))
                return DL_MAP_SEGMENTS_ERROR_MAP_ZERO_FILL;
            }
        }

      ++c;
    }

  /* Notify ELF_PREFERRED_ADDRESS that we have to load this one
     fixed.  */
  ELF_FIXED_ADDRESS (loader, c->mapstart);

  return NULL;

#endif /* ! defined __ptr128__  */
}
