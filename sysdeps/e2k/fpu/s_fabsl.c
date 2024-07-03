/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  ПРОГРАММА  ВЫЧИСЛЯЕТ  АБСОЛЮТНОЕ ЗНАЧЕНИЕ ВЕЩЕСТВЕН-  %
%  ЧИСЛА.                                                %
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
