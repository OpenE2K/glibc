#include <string.h>
#undef memset


void *memset(void *mem, int c, size_t len)
{
    char *ac = (char*)mem;
    long long *al;
    long long  cc;

    /* ��������� ����� ��������� ������� ������.
     * ��� ���������� ������ ��������� ���� ������� ����� ������
     * ���� �� ������ 8. */
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

    /* ��������� ������������� ����� ����� */
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

        /* ����� ���������� �� ������� ����� ��������� �����.
           ��� ����� ���� �� ����� ������ ��� ����. */
        if (len & 8)
            *(long long*)ac  = cc;

        if (len & 4)
            *(int*)(ac + (len & 8)) = cc;

        if (len & 2)
            *(short*)(ac + (len & 12)) = cc;

        if (len & 1)
            *(ac + (len & 14)) = c;

        /* ��������� ���� �� 2*8 ���� */
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
