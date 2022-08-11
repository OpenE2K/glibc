/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВОЗВРАЩАЕТ АРГУМЕНТ, ОКРУГЛЕННЫЙ В      %
%  СТОРОНУ +Inf ДО ЦЕЛОГО ЗНАЧЕНИЯ.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ceil (DB X)
{
# if __iset__ <= 2

  _type_double_bits absx, rez;
  /* округляем число прибавлением 2^52 (на дробную часть
     не хватит точности). направление округления зависит
     от текущего режима. */
  rez.value = (X > 0) ? (X + DVAIN52) - DVAIN52
                      : (X - DVAIN52) + DVAIN52;
  absx.value = X;

  /* большие по модулю числа и так целые */
  if (X <= -DVAIN52 || X >= DVAIN52)
      return X;

  /* выбираем целое число не меньшее исходного */
  if (X > rez.value)
    rez.value += 1;
  rez.llong |= absx.llong & 0x8000000000000000LL;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fdtoifd 0x2,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

#ifndef __ceil
weak_alias (__ceil, ceil)
#endif
