/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВОЗВРАЩАЕТ  АРГУМЕНТ,  ОКРУГЛЕННЫЙ  ДО  %
%  ЦЕЛОГО  ЗНАЧЕНИЯ  В  СООТВЕТСТВИИ  С  УСТАНОВЛЕННЫМ   %
%  РЕЖИМОМ  ОКРУГЛЕНИЯ.                                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

LL __llrint (DB X)
{
    LL res;
#pragma asm_inline
    __asm ("fdtoid %1,%0" : "=r" (res) : "r" (X));
    return res;
}

weak_alias (__llrint, llrint)
#ifdef NO_LONG_DOUBLE
strong_alias (__llrint, __llrintl)
weak_alias (__llrint, llrintl)
#endif
