#include <stdlib.h>

void *
__memrchr (const void *s, int c_in, size_t n)
{
  const unsigned char *src = (const unsigned char *) s;
  unsigned char c = (unsigned char) c_in;

  while (n--) 
    {
      if (src[n] == c)
	return (void *) &src[n];
    }

  return NULL;
}

weak_alias (__memrchr, memrchr)
