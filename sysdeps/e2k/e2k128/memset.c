/* Copyright (c) 2009-2024 AO MCST.
   Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <string.h>
#undef memset


void *memset(void *mem, int c, size_t len)
{
    char *ac = (char*)mem;
    long long *al;
    long long  cc;

    /* маленькие блоки заполняем простым циклом.
     * для корректной работы основного кода граница здесь должна
     * быть не меньше 8. */
    if (len < 16)
    {
#pragma loop count (8)
        while (len--)
            *ac++ = c;

        return mem;
    }

    c &= 0xff;
    cc =  c | (c << 8);
    cc = cc | (cc << 16);
    cc = cc | (cc << 32);

    /* заполняем невыровненную часть блока */
    {
        int  diff = (8 - (int) (long) mem) & 7;
        al   = (long long*)(ac + diff);
        len -= diff;

        if (diff & 1)
            *ac = c;

        if (diff & 2)
            *(short*)((char*)al - (diff & 4) - 2) = cc;

        if (diff & 4)
            *(int*)((char*)al - 4) = cc;
    }

    {
        size_t i;
        size_t ll = (len >> 3) & ~1;
        ac = (char*)al + (ll << 3);

        /* чтобы компилятор не умничал сразу заполняем хвост.
           это может быть не очень хорошо для кеша. */
        if (len & 8)
            *(long long*)ac  = cc;

        if (len & 4)
            *(int*)(ac + (len & 8)) = cc;

        if (len & 2)
            *(short*)(ac + (len & 12)) = cc;

        if (len & 1)
            *(ac + (len & 14)) = c;

        /* заполняем тело по 2*8 байт */
#pragma loop count (1000)
        for (i = 0; i < ll; i += 2)
        {
            al[i] = cc;
            al[i + 1] = cc;
        }
    }

    return mem;
}

libc_hidden_builtin_def (memset)
