/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  πςοηςαννα  χωώισμρετ  αβσομΰτξοε ϊξαώεξιε χεύεστχεξ-  %
%  ώισμα.                                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

LD __fabsl (LD X)
{
    _type_long_double_bits absx;
    absx.value = X;
    absx.twolong.short0 &= 0x7FFF;
    return absx.value;
}

weak_alias (__fabsl, fabsl)
