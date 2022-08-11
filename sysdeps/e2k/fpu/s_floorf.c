/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ���������  ���������� ��������, ����������� �      %
%  ������� -Inf �� ������ ��������.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __floorf (FL X)
{
# if __iset__ <= 2

  _type_float_bits absx, rez;
  /* ��������� ����� ������������ 2^23 (�� ������� �����
     �� ������ ��������). ����������� ���������� �������
     �� �������� ������. */
  rez.value = (X > 0) ? (X + DVAIN23) - DVAIN23
                      : (X - DVAIN23) + DVAIN23;

  absx.value = X;

  /* ������� �� ������ ����� � ��� ����� */
  if (X <= -DVAIN23 || X >= DVAIN23)
    return X;

  /* �������� ����� ����� �� ������� ��������� */
  if (rez.value > X)
    rez.value -= 1;
  rez.int0 |= absx.int0 & 0x80000000;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fstoifs 0x1,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

#ifndef __floorf
weak_alias (__floorf, floorf)
#endif
