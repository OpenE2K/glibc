/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ���������  ���������� ��������, ����������� �      %
%  ������� -Inf �� ������ ��������.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

LD __floorl (LD X)
{
  _type_long_double_bits absx, rez;

  /* ������� �� ������ ����� � ��� ����� */
  if (X <= -DVAIN63 || X >= DVAIN63)
    return X;

  /* ��������� ����� ������������ 2^63 (�� ������� �����
     �� ������ ��������). ����������� ���������� �������
     �� �������� ������. */
  rez.value = (X > 0) ? (X + DVAIN63) - DVAIN63
                      : (X - DVAIN63) + DVAIN63;
  absx.value = X;

  /* �������� ����� ����� �� ������� ��������� */
  if (rez.value > X)
    rez.value -= 1;
  rez.twolong.short0 |= absx.twolong.short0 & 0x8000;
  return rez.value;
}

weak_alias (__floorl, floorl)
