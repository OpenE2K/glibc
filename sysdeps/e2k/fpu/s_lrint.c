/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ртпзтбннб  чпъчтбэбеф  бтзхнеоф,  плтхзмеоощк  дп  %
%  гемпзп  ъобюеойс  ч  уппфчефуфчйй  у  хуфбопчмеоощн   %
%  тецйнпн  плтхзмеойс.                                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

LI __lrint (DB X)
{
    LI res;

#ifdef __ptr32__

#pragma asm_inline
    __asm ("fdtois %1,%0" : "=r" (res) : "r" (X));

#else /* __ptr32__ */

#pragma asm_inline
    __asm ("fdtoid %1,%0" : "=r" (res) : "r" (X));

#endif /* __ptr32__ */

    return res;
}

weak_alias (__lrint, lrint)
#ifdef NO_LONG_DOUBLE
strong_alias (__lrint, __lrintl)
weak_alias (__lrint, lrintl)
#endif
