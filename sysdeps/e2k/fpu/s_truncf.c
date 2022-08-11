/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ртпзтбннб  чпъчтбэбеф бтзхнеоф, плтхзмеоощк ч      %
%  уфптпох неошыезп рп нпдхма гемпзп ъобюеойс.           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __truncf (FL X)
{
# if __iset__ <= 2

  _type_float_bits absx, rez;

  if (!(X > -DVAIN23 && X < DVAIN23))
    return X;
  rez.value = (FL) (int) X;
  absx.value = X;
  rez.int0 |= absx.int0 & 0x80000000;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fstoifs 0x3,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

#ifndef __truncf
weak_alias (__truncf, truncf)
#endif
