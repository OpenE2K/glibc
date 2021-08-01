#include <string.h>
#include <stdlib.h>

size_t
__strnlen (const char *str, size_t maxlen)
{
  size_t i;

  for (i = 0; i < maxlen; i++)
    {
      if (str[i] == '\0')
	break;
    }

  return i;
}

libc_hidden_def (__strnlen)
weak_alias (__strnlen, strnlen)

libc_hidden_def (strnlen)
