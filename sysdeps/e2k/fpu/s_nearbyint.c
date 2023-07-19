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

DB __nearbyint (DB X)
{
# if __iset__ <= 2

  _type_double_bits absx, rez;
  unsigned int pfpfr;

  __fenv_getpfpfr (pfpfr);

  /* большие по модулю числа и так целые */
  if (X <= -DVAIN52 || X >= DVAIN52)
    return X;

  /* округляем число прибавлением 2^52 (на дробную часть
     не хватит точности). направление округления зависит
     от текущего режима. */
  rez.value = (X > 0) ? (X + DVAIN52) - DVAIN52
                      : (X - DVAIN52) + DVAIN52;
  __fenv_setpfpfr (pfpfr);
  absx.value = X;
  rez.llong |= absx.llong & 0x8000000000000000LL;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fdtoifd 0xc,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

weak_alias (__nearbyint, nearbyint)
