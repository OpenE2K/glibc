#include <sysdep.h>
#include <sys/utsname.h>

int
__uname (struct utsname *name)
{
  return INLINE_SYSCALL (uname, 1, name);
}

weak_alias (__uname, uname)
libc_hidden_def (__uname)
libc_hidden_def (uname)
