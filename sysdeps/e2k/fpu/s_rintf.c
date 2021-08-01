/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВОЗВРАЩАЕТ  АРГУМЕНТ,  ОКРУГЛЕННЫЙ  ДО  %
%  ЦЕЛОГО  ЗНАЧЕНИЯ  В  СООТВЕТСТВИИ  С  УСТАНОВЛЕННЫМ   %
%  РЕЖИМОМ  ОКРУГЛЕНИЯ.                                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __rintf (FL X)
{
# if __iset__ <= 2

  _type_float_bits absx, rez;
  /* округляем число прибавлением 2^23 (на дробную часть
     не хватит точности). направление округления зависит
     от текущего режима. */
  rez.value = (X > 0) ? (X + DVAIN23) - DVAIN23
                      : (X - DVAIN23) + DVAIN23;
  absx.value = X;
  rez.int0 |= absx.int0 & 0x80000000;

  /* большие по модулю числа и так целые */
  if (X <= -DVAIN23 || X >= DVAIN23)
    return X;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fstoifs 0x4,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

#ifndef __rintf
weak_alias (__rintf, rintf)
#endif
