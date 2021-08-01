/*
 * fpclassify(x) returns classification value corresponding to argument;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include "math.h"
#include "math_private.h"

int
__fpclassify (double x)
{
  __extension__ union { double __f; long long __i; } __xx = { __f: x };
  int __ex, __retval = FP_NORMAL;
  long long __m;

  __xx.__i &= ~0x8000000000000000LL;
  __ex = __xx.__i >> 52;
  __m = __xx.__i & 0xfffffffffffffLL;
  if (__ex == 0)
    __retval = __m == 0 ? FP_ZERO : FP_SUBNORMAL;
  else if (__ex == 0x7ff)
    __retval = __m == 0 ? FP_INFINITE : FP_NAN;
  return __retval;
}

hidden_def (__fpclassify)
#ifdef NO_LONG_DOUBLE
strong_alias (__fpclassify, __fpclassifyl)
#endif
