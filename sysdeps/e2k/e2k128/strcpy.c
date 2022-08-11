#include <stddef.h>
#include <string.h>

#undef strcpy

#ifndef STRCPY
# define STRCPY strcpy
#endif

/* Copy SRC to DEST.  */
char *
STRCPY (char *dest, const char *src)
{
  char *s = dest;
  int i = 0;

  while(dest[i] = src[i])
    i++;

  return s;
}

libc_hidden_builtin_def (strcpy)
