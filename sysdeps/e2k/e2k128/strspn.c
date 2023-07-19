#include <string.h>
#include <stdint.h>
#include <libc-pointer-arith.h>

#undef strspn
#ifndef STRSPN
# define STRSPN strspn
#endif

/* Return the length of the maximum initial segment
   of S which contains only characters in ACCEPT.  */
size_t
STRSPN (const char *str, const char *accept)
{
  const char *s = str;

  while (*s)
    {
      const  char *c;
      for (c = accept; *c; c++)
	{
	  if (*s == *c)
	    break;
	}

      if (*c == '\0')
	break;

      s++;
    }

  return (s - str);
}
libc_hidden_builtin_def (strspn)
