/* Return list with names for address in backtrace.
   Copyright (C) 1998-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1998.

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

#include <assert.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ldsodefs.h>

#if __ELF_NATIVE_CLASS == 32
# define WORD_WIDTH 8
#else
/* We assume 64bits.  */
# define WORD_WIDTH 16
#endif


char **
__backtrace_symbols (void *const *array, int size)
{
  Dl_info info[size];
  int status[size];
  int cnt;
  size_t total = 0;
  char **result;
#if defined __e2k__
  const char signal_handler_called[] = "<signal handler called>";
#endif /* __e2k__  */

  /* Fill in the information we can get from `dladdr'.  */
  for (cnt = 0; cnt < size; ++cnt)
    {
      struct link_map *map;
      status[cnt] = _dl_addr (array[cnt], &info[cnt], &map, NULL);
      if (status[cnt] && info[cnt].dli_fname && info[cnt].dli_fname[0] != '\0')
	{
	  /* We have some info, compute the length of the string which will be
	     "<file-name>(<sym-name>+offset) [address].  */
	  total += (strlen (info[cnt].dli_fname ?: "")
		    + strlen (info[cnt].dli_sname ?: "")
		    + 3 + WORD_WIDTH + 3 + WORD_WIDTH + 5);

	  /* The load bias is more useful to the user than the load
	     address.  The use of these addresses is to calculate an
	     address in the ELF file, so its prelinked bias is not
	     something we want to subtract out.  */
#if !defined __ptr128__
	  info[cnt].dli_fbase = (void *) map->l_addr;
#else /* defined __ptr128__  */
	  /* TODO: find out why they prefer to set `dli_fbase' to L_ADDR for
	     ordinary modes above unlike dl-addr.c where it's set to
	     L_MAP_START. I try to stupidly imitate their behaviour for now. */
	  info[cnt].dli_tbase = map->l_code_addr;
	  info[cnt].dli_dbase = map->l_addr;
#endif /* defined __ptr128__  */
	}
#if defined __e2k__
      else if (array[cnt] == (void *) -1UL)
        total += sizeof (signal_handler_called);
#endif /* __e2k__  */
      else
	total += 5 + WORD_WIDTH;
    }

  /* Allocate memory for the result.  */
  result = (char **) malloc (size * sizeof (char *) + total);
  if (result != NULL)
    {
      char *last = (char *) (result + size);

      for (cnt = 0; cnt < size; ++cnt)
	{
	  result[cnt] = last;

	  if (status[cnt]
	      && info[cnt].dli_fname != NULL && info[cnt].dli_fname[0] != '\0')
	    {
	      if (info[cnt].dli_sname == NULL)
		{
		  /* We found no symbol name to use, so describe it as
		     relative to the file.  */
#if !defined __ptr128__
		  info[cnt].dli_saddr = info[cnt].dli_fbase;
#else /* defined __ptr128__  */
		  /* FIXME: stupidly make use of dli_tbase here for now since
		     backtrace addresses are likely to belong to CUD. However,
		     some sort of test should be probably implemented to ensure
		     that this is the case.  */
		  info[cnt].dli_saddr = info[cnt].dli_tbase;
#endif /* defined __ptr128__  */
		}

	      if (info[cnt].dli_sname == NULL && info[cnt].dli_saddr == 0)
		last += 1 + sprintf (last, "%s(%s) [%p]",
				     info[cnt].dli_fname ?: "",
				     info[cnt].dli_sname ?: "",
				     array[cnt]);
	      else
		{
		  char sign;
		  ptrdiff_t offset;
		  if (array[cnt] >= (void *) info[cnt].dli_saddr)
		    {
		      sign = '+';
		      offset = (
#if defined __ptr128__
				(ptrdiff_t)
#endif
				array[cnt] - info[cnt].dli_saddr);
		    }
		  else
		    {
		      sign = '-';
		      offset = (info[cnt].dli_saddr
				-
#if defined __ptr128__
				(ptrdiff_t)
#endif /* defined __ptr128__  */
				array[cnt]);
		    }

		  last += 1 + sprintf (last, "%s(%s%c%#tx) [%p]",
				       info[cnt].dli_fname ?: "",
				       info[cnt].dli_sname ?: "",
				       sign, offset, array[cnt]);
		}
	    }
#if defined __e2k__
          else if (array[cnt] == (void *) -1UL)
            /* `1 + ' accounts for the trailing '\0'. */
            last += 1 + sprintf (last, "%s", signal_handler_called);
#endif /* __e2k__  */
	  else
	    last += 1 + sprintf (last, "[%p]", array[cnt]);
	}
      assert (last <= (char *) result + size * sizeof (char *) + total);
    }

  return result;
}
weak_alias (__backtrace_symbols, backtrace_symbols)
