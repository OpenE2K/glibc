#include <string.h>
#include <memcopy.h>

#if     !defined(RETURN)
#undef memcpy
#define RETURN(p,shift) return (void*)(p) /* Return dst */
#endif

#define restrict __restrict__

#define __builtin_e2k_gettags(__src) \
  ({ \
    unsigned _res; \
    _Pragma("asm_inline") \
    asm ("gettags %1, %0" : "=r"(_res) : "r"(__src)); \
    _res; \
  })

#define __builtin_e2k_puttags(__src) \
  ({ \
    unsigned _res; \
    _Pragma("asm_inline") \
    asm ("puttags %1, 0x0, %0" : "=r"(_res) : "r"(__src)); \
    _res; \
  })


void *
memcpy (void *out, const void *in, size_t length)
{
    char       * restrict char_dst = (char*)out;
    char       * restrict char_src = (char*)in;
    size_t      ba_dst = (size_t)out, ba_src = (size_t)in;
    size_t      base_dst, base_src;
    size_t      len = length;

#define DO_COPY(T)                                         \
    {                                                      \
        size_t ii  = len / sizeof(T);                      \
        size_t msk = sizeof(T) - 1;                        \
        if ((ba_dst&msk)==0 && (ba_src&msk)==0)            \
        {                                                  \
_Pragma ("loop count (1000)")                              \
                while (ii--)                               \
                {                                          \
                        *((T*)char_dst) = *((T*)char_src); \
                        char_dst += sizeof(T);             \
                        char_src += sizeof(T);             \
                }                                          \
                                                           \
                len &= msk;                                \
                if (!len) RETURN (out, length);            \
        }                                                  \
    }

    /* у нас 128-битные указатели */
    /* assert(sizeof(char*) > sizeof(long long)); */

    /* копируем начало по байтам до выровненного на 4 адреса */
#pragma loop count(2)
    while ((((size_t) char_dst) & 3) && len) {
        *(char_dst++) = *(char_src++);
        len--;
    }

    /* выровненность адресов на 4 разная, придется извращаться с сохранением тегов */
    if ((ba_dst ^ ba_src) & 3) {
        long long ii, align = (size_t) char_src & 3;
        unsigned s0, s1, tag0, tag1;

        /* выровненный src выходит за левую границу */
        if (char_src < ((char*) in + align)) {
            /* копируем еще 4 байта */
#pragma loop count(4)
            for (ii = 4; ii && len; ii--, len--)
                *(char_dst++) = *(char_src++);
        }
        ii = (long long) (len - 4 + align) >> 2;
        if (ii > 0) {
            s0 = ((unsigned *) (char_src - align))[0];
            tag0 = __builtin_e2k_gettags (s0);
            len -= ii << 2;
        }

#pragma loop count(1000)
        while (ii-- > 0) { /* основной цикл по 4 байта */
            s1 = ((unsigned *) (char_src - align))[1];
            tag1 = __builtin_e2k_gettags (s1);

            if (tag0 == 0 || tag1 == 0) {
                if (tag0) s0 = 0; /* на случай, чтобы биты диагностики не попали в dst */
                s0 = (s0 >> (align * 8)) | (__builtin_e2k_puttags (s1) << (32 - align * 8));
            }
            *((unsigned *)char_dst) = s0;
            s0 = s1;
            tag0 = tag1;
            char_dst += 4;
            char_src += 4;
        }
        /* копируем хвост по байтам */
#pragma loop count(2)
        while (len--)
            *(char_dst++) = *(char_src++);
        RETURN (out, length);
    }

    /* выравниваем адрес dst на 16 байт */
    if ((((size_t) char_dst) & 4) && len > 3) {
        *((int *) char_dst) = *((int *) char_src);
        char_src += 4;
        char_dst += 4;
        len -= 4;
    }
    if ((((size_t) char_dst) & 8) && len > 7) {
        *((int *) char_dst) = *((int *) char_src);
        *((int *) char_dst + 1) = *((int *) char_src + 1);
        char_src += 8;
        char_dst += 8;
        len -= 8;
    }

    ba_dst = (size_t)char_dst;
    ba_src = (size_t)char_src;

    /* пробуем копировать по 128 бит */
    /* для копирования квадро словами нужно так же чтобы базовый (не суммарный)
       адрес дескриптора был кратен восьми (честное слово). */
    base_src = __builtin_e2k_get_ap_base(in);
    base_dst = __builtin_e2k_get_ap_base(out);
    if ((base_src & 7) == 0 && (base_dst & 7) == 0)
        DO_COPY(char*);

    /* пробуем копировать по 64 бита */
    DO_COPY(long long);

    /* пробуем копировать по 32 бита */
    DO_COPY(int);

    /* копируем хвост по байтам */
#pragma loop count(2)
    while (len--)
        *(char_dst++) = *(char_src++);
    RETURN (out, length);
}

libc_hidden_builtin_def (memcpy)
