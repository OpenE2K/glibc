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

LD __nearbyintl (LD X)
{
  _type_long_double_bits absx, rez;
  unsigned int fpsr;

  __fenv_getfpsr (fpsr);

  /* большие по модулю числа и так целые */
  if (X <= -DVAIN63 || X >= DVAIN63)
    return X;

  /* округляем число прибавлением 2^63 (на дробную часть
     не хватит точности). направление округления зависит
     от текущего режима. */
  rez.value = (X > 0) ? (X + DVAIN63) - DVAIN63
                      : (X - DVAIN63) + DVAIN63;
  __fenv_setfpsr (fpsr);
  absx.value = X;
  rez.twolong.short0 |= absx.twolong.short0 & 0x8000;
  return rez.value;
}

weak_alias (__nearbyintl, nearbyintl)
