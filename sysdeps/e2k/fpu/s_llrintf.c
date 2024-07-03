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

LL __llrintf (FL X)
{
    LL res;
#pragma asm_inline
    __asm ("fstoid %1,%0" : "=r" (res) : "r" (X));
    return res;
}

weak_alias (__llrintf, llrintf)

