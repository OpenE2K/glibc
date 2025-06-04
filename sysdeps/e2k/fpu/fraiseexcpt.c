/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Raise given exceptions. */

#include <fenv.h>
#include <float.h>
#include <math.h>
#include <shlib-compat.h>
#include <sysdep.h>

int ATTRIBUTE_NOINLINE_WHOLE
__feraiseexcept (int excepts)
{
  static volatile double sink;
  static const struct {
    double zero, one, max, min, pi;
  } c = {
    0.0, 1.0, DBL_MAX, DBL_MIN, M_PI
  };

  /* Raise exceptions represented by EXPECTS.  But we must raise only
     one signal at a time.  It is important the if the overflow/underflow
     exception and the inexact exception are given at the same time,
     the overflow/underflow exception follows the inexact exception.  */

  /* First: invalid exception.  */
  if ((FE_INVALID & excepts) != 0)
    /* One example of a invalid operation is 0/0.  */
    sink = c.zero / c.zero;

  /* Next: division by zero.  */
  if ((FE_DIVBYZERO & excepts) != 0)
    sink = c.one / c.zero;

  /* Next: overflow.  */
  if ((FE_OVERFLOW & excepts) != 0)
    sink = c.max * c.max;

  /* Next: underflow.  */
  if ((FE_UNDERFLOW & excepts) != 0)
    sink = c.min * c.min;

  /* Last: inexact.  */
  if ((FE_INEXACT & excepts) != 0)
    sink = c.one / c.pi;

#if ! defined __LCC__
  /* VOLATILE is not enough to prevent GCC from producing "variable set but
     not used" warning or may be even inappropriate for this purpose. Shut
     up it this way for now, but eventually look for a better way of making
     it clear that the evaluation of a variable has side effects.  */
  asm ("" : : "r" (&sink));
#endif /* ! defined __LCC__  */

  /* Success.  */
  return 0;
}

libm_hidden_def (__feraiseexcept)
libm_hidden_ver (__feraiseexcept, feraiseexcept)
versioned_symbol (libm, __feraiseexcept, feraiseexcept, GLIBC_2_2);
