/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВОЗВРАЩАЕТ АРГУМЕНТ, ОКРУГЛЕННЫЙ В      %
%  СТОРОНУ БЛИЖАЙШЕГО ЦЕЛОГО ЗНАЧЕНИЯ. ЗНАЧЕНИЯ ТОЧНО    %
%  ПОСРЕДИНЕ МЕЖДУ ДВУМЯ ЦЕЛЫМИ ЧИСЛАМИ ОКРУГЛЯЮТСЯ В    %
%  СТОРОНУ ОТ 0 (УВЕЛИЧЕНИЯ ПО МОДУЛЮ).                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

long int
__lroundf (FL X)
{
  _type_float_bits res, tmp;
  FL absx = ffabs (X);

  tmp.value = X;
  tmp.int0 &= 0x80000000;
  res.int0 = tmp.int0 | 0x3f000000; /* 0.5 со знаком X */

  /* большие по модулю числа и так целые */
  if (!(absx < DVAIN23))
    return (long int) X;

  if (absx < 0.5f) /* для X, равным почти 0.5 нельзя добавлять 0.5 */
    return 0;

  return (long int) (res.value + X);
}

weak_alias (__lroundf, lroundf)
