#include <string.h>
#include <memcopy.h>
#include <stdlib.h>

#undef strchr


char *
strchr (const char *s, int i)
{
  char c = i;

  while (*s && *s != c) 
    {
      s++;
    }

  if (*s != c) 
    {
      s = NULL;
    }

  return (char *) s;
}

#ifdef weak_alias
# undef index
weak_alias (strchr, index)
#endif
libc_hidden_builtin_def (strchr)
