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

  tmp.value = X;

# if __iset__ <= 2

  FL absx = ffabs (X);

  /* ΒΟΜΨΫΙΕ ΠΟ ΝΟΔΥΜΐ ήΙΣΜΑ Ι ΤΑΛ ΓΕΜΩΕ */
  if (!(absx < DVAIN23))
    return X;

  res.value = (FL) (int) (absx + 0x1.fffffep-2f); /* |x| + ΠΟήΤΙ 0.5 */
  res.int0 |= tmp.int0 & 0x80000000;

# else /* __iset__ <= 2 */

  res.value = 0x1.fffffep-2f;        /* ΠΟήΤΙ 0.5 */
  res.int0 |= tmp.int0 & 0x80000000;

  res.value += X;
#pragma asm_inline
  __asm ("fstoifs 0x3,%0,%0" : "+r" (res.value)); /* ΟΤΒÒΑΣΩΧΑΞΙΕ ΔÒΟΒΞΟΚ ήΑΣΤΙ */

# endif /* __iset__ <= 2 */

  return res.value;
}

weak_alias (__roundf, roundf)
