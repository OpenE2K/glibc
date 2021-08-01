#include <execinfo.h>

int
__backtrace (void **array, int size)
{
  return 0;
}

weak_alias (__backtrace, backtrace)
libc_hidden_def (__backtrace)
