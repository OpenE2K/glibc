#include <errno.h>
#include <stdarg.h>
#include <lowlevellock-futex.h>


int
futex (void *uaddr, int futex_op, int val, ...)
{
  int res = -ENOSYS;

  if (futex_op == FUTEX_WAKE)
    res = lll_futex_syscall (4, uaddr, futex_op, val, 0);
  else if (futex_op == FUTEX_WAIT)
    {
      va_list ap;
      void *timeout;
      int res;

      va_start (ap, val);
      timeout = va_arg (ap, void *);
      res = lll_futex_syscall (4, uaddr, futex_op, val, timeout);
      va_end (ap);
    }

  if (res != 0)
    {
      errno = -res;
      res = -1;
    }

  return res;
}
