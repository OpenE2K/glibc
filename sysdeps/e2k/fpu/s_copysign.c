/* Copyright (c) 2019 ZAO "MCST". All rights reserved. */

/*
 * copysign(double x, double y)
 * copysign(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#define NO_MATH_REDIRECT
#include <math.h>
#include <math_private.h>
#include <libm-alias-double.h>

double __copysign (double x, double y)
{
  return __builtin_copysign (x, y);
}

libm_alias_double (__copysign, copysign)
