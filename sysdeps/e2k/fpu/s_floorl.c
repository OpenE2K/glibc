/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВОЗВРАЩАЕТ АРГУМЕНТ, ОКРУГЛЕННЫЙ В      %
%  СТОРОНУ -Inf ДО ЦЕЛОГО ЗНАЧЕНИЯ.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

LD __floorl (LD X)
{
  _type_long_double_bits absx, rez;

  /* большие по модулю числа и так целые */
  if (X <= -DVAIN63 || X >= DVAIN63)
    return X;

  /* округляем число прибавлением 2^63 (на дробную часть
     не хватит точности). направление округления зависит
     от текущего режима. */
  rez.value = (X > 0) ? (X + DVAIN63) - DVAIN63
                      : (X - DVAIN63) + DVAIN63;
  absx.value = X;

  /* выбираем целое число не большее исходного */
  if (rez.value > X)
    rez.value -= 1;
  rez.twolong.short0 |= absx.twolong.short0 & 0x8000;
  return rez.value;
}

weak_alias (__floorl, floorl)
