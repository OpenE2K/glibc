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

LI __lrintf (FL X)
{
    LI res;

#ifdef __ptr32__

#pragma asm_inline
    __asm ("fstois %1,%0" : "=r" (res) : "r" (X));

#else /* __ptr32__ */

#pragma asm_inline
    __asm ("fstoid %1,%0" : "=r" (res) : "r" (X));

#endif /* __ptr32__ */

    return res;
}

weak_alias (__lrintf, lrintf)
