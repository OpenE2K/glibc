/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ���������  ���������� ��������, ����������� �      %
%  ������� ���������� ������ ��������. �������� �����    %
%  ��������� ����� ����� ������ ������� ����������� �    %
%  ������� �� 0 (���������� �� ������).                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

long int
__lroundf (FL X)
{
  _type_float_bits res, tmp;
  FL absx = ffabs (X);

  tmp.value = X;
  tmp.int0 &= 0x80000000;
  res.int0 = tmp.int0 | 0x3f000000; /* 0.5 �� ������ X */

  /* ������� �� ������ ����� � ��� ����� */
  if (!(absx < DVAIN23))
    return (long int) X;

  if (absx < 0.5f) /* ��� X, ������ ����� 0.5 ������ ��������� 0.5 */
    return 0;

  return (long int) (res.value + X);
}

weak_alias (__lroundf, lroundf)
