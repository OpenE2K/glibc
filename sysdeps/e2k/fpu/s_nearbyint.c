/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <fenv.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ���������  ����������  ��������,  �����������  ��  %
%  ������  ��������  �  ������������  �  �������������   %
%  �������  ����������. �� ������������� ���� INEXACT.   %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __nearbyint (DB X)
{
# if __iset__ <= 2

  _type_double_bits absx, rez;
  unsigned int pfpfr;

  __fenv_getpfpfr (pfpfr);

  /* ������� �� ������ ����� � ��� ����� */
  if (X <= -DVAIN52 || X >= DVAIN52)
    return X;

  /* ��������� ����� ������������ 2^52 (�� ������� �����
     �� ������ ��������). ����������� ���������� �������
     �� �������� ������. */
  rez.value = (X > 0) ? (X + DVAIN52) - DVAIN52
                      : (X - DVAIN52) + DVAIN52;
  __fenv_setpfpfr (pfpfr);
  absx.value = X;
  rez.llong |= absx.llong & 0x8000000000000000LL;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fdtoifd 0xc,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

weak_alias (__nearbyint, nearbyint)
