/* Copyright (c) 2019 ZAO "MCST". All rights reserved. */

/*
 * copysignf(float x, float y)
 * copysignf(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#define NO_MATH_REDIRECT
#include <math.h>
#include <math_private.h>
#include <libm-alias-float.h>

float __copysignf (float x, float y)
{
  return __builtin_copysignf (x, y);
}

libm_alias_float (__copysign, copysign)
