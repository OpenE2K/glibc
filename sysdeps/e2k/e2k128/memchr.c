#ifndef _LIBC
# include <config.h>
#endif

#include <string.h>

#include <stddef.h>

#include <limits.h>

#undef __memchr
#ifdef _LIBC
# undef memchr
#endif

#ifndef weak_alias
# define __memchr memchr
#endif

#ifndef MEMCHR
# define MEMCHR __memchr
#endif

/* Search no more than N bytes of S for C.  */
void *
MEMCHR (void const *s, int c_in, size_t n)
{
  const unsigned char *src = (const unsigned char *) s;
  unsigned char c = (unsigned char) c_in;

  while (n--) 
    {
      if (*src == c)
	return (char *) src;
      src++;
    }

  return NULL;
}

#ifdef weak_alias
weak_alias (__memchr, memchr)
#endif
libc_hidden_builtin_def (memchr)
