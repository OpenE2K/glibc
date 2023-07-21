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
__lround (DB X)
{
  _type_double_bits res, tmp;
  DB absx = dfabs (X);

  tmp.value = X;
  tmp.llong &= 0x8000000000000000LL;
  res.llong = tmp.llong | 0x3fe0000000000000LL; /* 0.5 �� ������ X */

  /* ������� �� ������ ����� � ��� ����� */
  if (!(absx < DVAIN52))
    return (long int) X;

  if (absx < 0.5) /* ��� X, ������ ����� 0.5 ������ ��������� 0.5 */
    return 0;

  return (long int) (res.value + X);
}

weak_alias (__lround, lround)
#ifdef NO_LONG_DOUBLE
strong_alias (__lround, __lroundl)
weak_alias (__lround, lroundl)
#endif
