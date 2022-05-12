/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#define NO_MATH_REDIRECT
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χοϊχςαύαετ αςηυνεξτ, ολςυημεξξωκ χ      %
%  στοςοξυ βμιφακϋεηο γεμοηο ϊξαώεξιρ. ϊξαώεξιρ τοώξο    %
%  ποσςεδιξε νεφδυ δχυνρ γεμωνι ώισμανι ολςυημρΰτσρ χ    %
%  στοςοξυ οτ 0 (υχεμιώεξιρ πο νοδυμΰ).                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __roundf (FL X)
{
  _type_float_bits res, tmp;
  FL absx = ffabs (X);

  tmp.value = X;
  tmp.int0 &= 0x80000000;
  res.int0 = tmp.int0 | 0x3f000000; /* 0.5 ΣΟ ΪΞΑΛΟΝ X */
  res.value += X;

  /* ΒΟΜΨΫΙΕ ΠΟ ΝΟΔΥΜΐ ήΙΣΜΑ Ι ΤΑΛ ΓΕΜΩΕ */
  if (!(absx < DVAIN23))
    return X;

  if (absx < 0.5f) /* ΔΜΡ X, ÒΑΧΞΩΝ ΠΟήΤΙ 0.5 ΞΕΜΨΪΡ ΔΟΒΑΧΜΡΤΨ 0.5 */
    return tmp.value;

# if __iset__ <= 2

  res.value = (FL) (int) res.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fstoifs 0x3,%0,%0" : "+r" (res.value)); /* ΟΤΒÒΑΣΩΧΑΞΙΕ ΔÒΟΒΞΟΚ ήΑΣΤΙ */

# endif /* __iset__ <= 2 */

  return res.value;
}

weak_alias (__roundf, roundf)
