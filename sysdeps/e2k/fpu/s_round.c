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

  tmp.value = X;

# if __iset__ <= 2

  DB absx = dfabs (X);

  /* ΒΟΜΨΫΙΕ ΠΟ ΝΟΔΥΜΐ ήΙΣΜΑ Ι ΤΑΛ ΓΕΜΩΕ */
  if (!(absx < DVAIN52))
    return X;

  absx += 0x1.fffffffffffffp-2; /* |x| + ΠΟήΤΙ 0.5 */
  res.value = (DB) (LL) absx;
  res.llong |= tmp.llong & 0x8000000000000000LL;

# else /* __iset__ <= 2 */

  res.value = 0x1.fffffffffffffp-2; /* ΠΟήΤΙ 0.5 */
  res.llong |= tmp.llong & 0x8000000000000000LL;

  res.value += X;
#pragma asm_inline
  __asm ("fdtoifd 0x3,%0,%0" : "+r" (res.value)); /* ΟΤΒÒΑΣΩΧΑΞΙΕ ΔÒΟΒΞΟΚ ήΑΣΤΙ */

# endif /* __iset__ <= 2 */

  return res.value;
}

weak_alias (__round, round)
