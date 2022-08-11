#include <string.h>

size_t
strlen (const char *str)
{
  const char *start = str;

  if (!start[0]) return 0;
  if (!start[1]) return 1;
  if (!start[2]) return 2;

  str += 3;
  while (*str++);

  return str - start - 1;
}

libc_hidden_builtin_def (strlen)
