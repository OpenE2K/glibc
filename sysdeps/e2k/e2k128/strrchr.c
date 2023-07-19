#include <string.h>

#undef strrchr

#ifndef STRRCHR
# define STRRCHR strrchr
#endif


char *
STRRCHR (const char *s, int i)
{
  const char *last = NULL;
  char c = i;

  while (*s)
    {
      if (*s == c)
	{
	  last = s;
	}

      s++;
    }

  if (*s == c)
    {
      last = s;
    }


  return (char *) last;
}

#ifdef weak_alias
# undef rindex
weak_alias (strrchr, rindex)
#endif
libc_hidden_builtin_def (strrchr)
