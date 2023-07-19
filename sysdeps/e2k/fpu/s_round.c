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

DB __round (DB X)
{
  _type_double_bits res, tmp;
  DB absx = dfabs (X);

  tmp.value = X;
  tmp.llong &= 0x8000000000000000LL;
  res.llong = tmp.llong | 0x3fe0000000000000LL; /* 0.5 ΣΟ ΪΞΑΛΟΝ X */
  res.value += X;

  /* ΒΟΜΨΫΙΕ ΠΟ ΝΟΔΥΜΐ ήΙΣΜΑ Ι ΤΑΛ ΓΕΜΩΕ */
  if (!(absx < DVAIN52))
    return X;

  if (absx < 0.5) /* ΔΜΡ X, ÒΑΧΞΩΝ ΠΟήΤΙ 0.5 ΞΕΜΨΪΡ ΔΟΒΑΧΜΡΤΨ 0.5 */
    return tmp.value;

# if __iset__ <= 2

  res.value = (DB) (LL) res.value;

# else /* __iset__ <= 2 */

#pragma asm_inline
  __asm ("fdtoifd 0x3,%0,%0" : "+r" (res.value)); /* ΟΤΒÒΑΣΩΧΑΞΙΕ ΔÒΟΒΞΟΚ ήΑΣΤΙ */

# endif /* __iset__ <= 2 */

  return res.value;
}

weak_alias (__round, round)
