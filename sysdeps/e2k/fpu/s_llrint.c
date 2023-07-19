/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ртпзтбннб  чпъчтбэбеф  бтзхнеоф,  плтхзмеоощк  дп  %
%  гемпзп  ъобюеойс  ч  уппфчефуфчйй  у  хуфбопчмеоощн   %
%  тецйнпн  плтхзмеойс.                                  %
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
