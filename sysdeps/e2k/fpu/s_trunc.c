/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВОЗВРАЩАЕТ АРГУМЕНТ, ОКРУГЛЕННЫЙ В      %
%  СТОРОНУ МЕНЬШЕГО ПО МОДУЛЮ ЦЕЛОГО ЗНАЧЕНИЯ.           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __trunc (DB X)
{
# if __iset__ <= 2

  _type_double_bits absx, rez;

  /* большие по модулю числа и так целые */
  if (!(X > -DVAIN52 && X < DVAIN52))
    return X;
  rez.value = (DB) (LL) X;
  absx.value = X;
  rez.llong |= absx.llong & 0x8000000000000000LL;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fdtoifd 0x3,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

#ifndef __trunc
weak_alias (__trunc, trunc)
#endif
