/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ртпзтбннб  чпъчтбэбеф  бтзхнеоф,  плтхзмеоощк  дп  %
%  гемпзп  ъобюеойс  ч  уппфчефуфчйй  у  хуфбопчмеоощн   %
%  тецйнпн  плтхзмеойс.                                  %
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
