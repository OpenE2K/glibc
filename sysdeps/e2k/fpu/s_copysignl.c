/* Copyright (c) 2019 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/*
 * copysignl(long double x, long double y)
 * copysignl(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#define NO_MATH_REDIRECT
#include <math.h>
#include <math_private.h>
#include <libm-alias-ldouble.h>

long double __copysignl (long double x, long double y)
{
  return __builtin_copysignl (x, y);
}

libm_alias_ldouble (__copysign, copysign)
