#include <string.h>
#include <stdint.h>
#include <libc-pointer-arith.h>

#undef strcspn

#ifndef STRCSPN
# define STRCSPN strcspn
#endif

/* Return the length of the maximum initial segment of S
   which contains no characters from REJECT.  */
size_t
STRCSPN (const char *str, const char *reject)
{
  const char *s = str;
  const char *c;

  while (*s)
    {
      for (c = reject; *c; c++)
	{
	  if (*s == *c)
	    break;
	}

      if (*c)
	break;

      s++;
    }

  return s - str;
}

libc_hidden_builtin_def (strcspn)
