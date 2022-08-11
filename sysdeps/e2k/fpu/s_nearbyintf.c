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

FL __nearbyintf (FL X)
{
# if __iset__ <= 2

  _type_float_bits absx, rez;
  unsigned int pfpfr;

  __fenv_getpfpfr (pfpfr);

  /* ������� �� ������ ����� � ��� ����� */
  if (X <= -DVAIN23 || X >= DVAIN23)
    return X;

  /* ��������� ����� ������������ 2^23 (�� ������� �����
     �� ������ ��������). ����������� ���������� �������
     �� �������� ������. */
  rez.value = (X > 0) ? (X + DVAIN23) - DVAIN23
                      : (X - DVAIN23) + DVAIN23;
  __fenv_setpfpfr (pfpfr);
  absx.value = X;
  rez.int0 |= absx.int0 & 0x80000000;
  return rez.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fstoifs 0xc,%0,%0" : "+r" (X));
  return X;

# endif /* __iset__ <= 2 */
}

weak_alias (__nearbyintf, nearbyintf)
