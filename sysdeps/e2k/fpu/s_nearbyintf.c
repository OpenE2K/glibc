/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <fenv.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВОЗВРАЩАЕТ  АРГУМЕНТ,  ОКРУГЛЕННЫЙ  ДО  %
%  ЦЕЛОГО  ЗНАЧЕНИЯ  В  СООТВЕТСТВИИ  С  УСТАНОВЛЕННЫМ   %
%  РЕЖИМОМ  ОКРУГЛЕНИЯ. НЕ УСТАНАВЛИВАЕТ ФЛАГ INEXACT.   %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __nearbyintf (FL X)
{
# if __iset__ <= 2

  _type_float_bits absx, rez;
  unsigned int pfpfr;

  __fenv_getpfpfr (pfpfr);

  /* большие по модулю числа и так целые */
  if (X <= -DVAIN23 || X >= DVAIN23)
    return X;

  /* округляем число прибавлением 2^23 (на дробную часть
     не хватит точности). направление округления зависит
     от текущего режима. */
  rez.value = (X > 0) ? (X + DVAIN23) - DVAIN23
                      : (X - DVAIN23) + DVAIN23;
  __fenv_setpfpfr (pfpfr);
  absx.value = X;
  rez.int0 |= absx.int0 & 0x80000000;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fstoifs 0xc,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

weak_alias (__nearbyintf, nearbyintf)
