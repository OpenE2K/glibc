/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χοϊχςαύαετ αςηυνεξτ, ολςυημεξξωκ χ      %
%  στοςοξυ βμιφακϋεηο γεμοηο ϊξαώεξιρ. ϊξαώεξιρ τοώξο    %
%  ποσςεδιξε νεφδυ δχυνρ γεμωνι ώισμανι ολςυημρΰτσρ χ    %
%  στοςοξυ οτ 0 (υχεμιώεξιρ πο νοδυμΰ).                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

long long int
__llround (DB X)
{
  _type_double_bits res, tmp;
  DB absx = dfabs (X);

  tmp.value = X;
  tmp.llong &= 0x8000000000000000LL;
  res.llong = tmp.llong | 0x3fe0000000000000LL; /* 0.5 ΣΟ ΪΞΑΛΟΝ X */

  /* ΒΟΜΨΫΙΕ ΠΟ ΝΟΔΥΜΐ ήΙΣΜΑ Ι ΤΑΛ ΓΕΜΩΕ */
  if (!(absx < DVAIN52))
    return (LL) X;

  if (absx < 0.5) /* ΔΜΡ X, ÒΑΧΞΩΝ ΠΟήΤΙ 0.5 ΞΕΜΨΪΡ ΔΟΒΑΧΜΡΤΨ 0.5 */
    return 0;

  return (LL) (res.value + X);
}

weak_alias (__llround, llround)
#ifdef NO_LONG_DOUBLE
strong_alias (__llround, __llroundl)
weak_alias (__llround, llroundl)
#endif
