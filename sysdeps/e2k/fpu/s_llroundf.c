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
__llroundf (FL X)
{
  _type_float_bits res, tmp;
  FL absx = ffabs (X);

  tmp.value = X;
  tmp.int0 &= 0x80000000;
  res.int0 = tmp.int0 | 0x3f000000; /* 0.5 ΣΟ ΪΞΑΛΟΝ X */

  /* ΒΟΜΨΫΙΕ ΠΟ ΝΟΔΥΜΐ ήΙΣΜΑ Ι ΤΑΛ ΓΕΜΩΕ */
  if (!(absx < DVAIN23))
    return (LL) X;

  if (absx < 0.5f) /* ΔΜΡ X, ÒΑΧΞΩΝ ΠΟήΤΙ 0.5 ΞΕΜΨΪΡ ΔΟΒΑΧΜΡΤΨ 0.5 */
    return 0;

  return (LL) (res.value + X);
}

weak_alias (__llroundf, llroundf)
