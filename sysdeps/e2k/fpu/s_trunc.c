/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ртпзтбннб  чпъчтбэбеф бтзхнеоф, плтхзмеоощк ч      %
%  уфптпох неошыезп рп нпдхма гемпзп ъобюеойс.           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __trunc (DB X)
{
# if __iset__ <= 2

  _type_double_bits absx, rez;

  /* ВПМШЫЙЕ РП НПДХМА ЮЙУМБ Й ФБЛ ГЕМЩЕ */
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
