/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ртпзтбннб  чпъчтбэбеф  бтзхнеоф,  плтхзмеоощк  дп  %
%  гемпзп  ъобюеойс  ч  уппфчефуфчйй  у  хуфбопчмеоощн   %
%  тецйнпн  плтхзмеойс.                                  %
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

