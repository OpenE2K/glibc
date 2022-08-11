/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ���������  ���������� ��������, ����������� �      %
%  ������� ���������� ������ ��������. �������� �����    %
%  ��������� ����� ����� ������ ������� ����������� �    %
%  ������� �� 0 (���������� �� ������).                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __roundf (FL X)
{
  _type_float_bits res, tmp;
  FL absx = ffabs (X);

  tmp.value = X;
  tmp.int0 &= 0x80000000;
  res.int0 = tmp.int0 | 0x3f000000; /* 0.5 �� ������ X */
  res.value += X;

  /* ������� �� ������ ����� � ��� ����� */
  if (!(absx < DVAIN23))
    return X;

  if (absx < 0.5f) /* ��� X, ������ ����� 0.5 ������ ��������� 0.5 */
    return tmp.value;

# if __iset__ <= 2

  res.value = (FL) (int) res.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fstoifs 0x3,%0,%0" : "+r" (res.value)); /* ������������ ������� ����� */

# endif /* __iset__ <= 2 */

  return res.value;
}

weak_alias (__roundf, roundf)
