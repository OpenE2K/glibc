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

DB __round (DB X)
{
  _type_double_bits res, tmp;
  DB absx = dfabs (X);

  tmp.value = X;
  tmp.llong &= 0x8000000000000000LL;
  res.llong = tmp.llong | 0x3fe0000000000000LL; /* 0.5 �� ������ X */
  res.value += X;

  /* ������� �� ������ ����� � ��� ����� */
  if (!(absx < DVAIN52))
    return X;

  if (absx < 0.5) /* ��� X, ������ ����� 0.5 ������ ��������� 0.5 */
    return tmp.value;

# if __iset__ <= 2

  res.value = (DB) (LL) res.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fdtoifd 0x3,%0,%0" : "+r" (res.value)); /* ������������ ������� ����� */

# endif /* __iset__ <= 2 */

  return res.value;
}

weak_alias (__round, round)
