#include <string.h>

char *
__strchrnul (const char *s, int i)
{
  char c = i;

  while (*s)
    {
      if (*s == c)
	return (char *) s;

      s++;
    }

  return (char *) s;
}

weak_alias (__strchrnul, strchrnul)
