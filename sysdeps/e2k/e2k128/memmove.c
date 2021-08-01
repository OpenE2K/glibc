#include <string.h>
#include <memcopy.h>

/* All this is so that bcopy.c can #include
   this file after defining some things.  */
#ifndef	a1
#define	a1	dst	/* First arg is DEST.  */
#define	a1const
#define	a2	src	/* Second arg is SRC.  */
#define	a2const	const
#undef memmove
#endif
#if	!defined(RETURN) || !defined(rettype)
#define	RETURN(s)	return (s)	/* Return DEST.  */
#define	rettype		void *
#endif

#ifndef MEMMOVE
#define MEMMOVE memmove
#endif

rettype
inhibit_loop_to_libcall
MEMMOVE (a1const void *a1, a2const void *a2, size_t len)
{
  void *dst_void = dst;
  const void *src_void = src;
  size_t length = len;

#define DO_COPY(T, MASK)                            \
  ii = length;					    \
  if ((ba_dst&(MASK))==0 && (ba_src&(MASK))==0)	    \
    ii &= (MASK);				    \
  if (ii)					    \
    {                                               \
      length -= ii;				    \
      ii /= sizeof(T);				    \
      while (ii--)				    \
        {                                           \
	  char_dst -= sizeof(T);		    \
	  char_src -= sizeof(T);		    \
	  *((T*)char_dst) = *((T*)char_src);	    \
        }                                           \
      						    \
      if (!length) RETURN (dst_void);		    \
    }

  size_t ba_dst = (size_t)dst_void;
  size_t ba_src = (size_t)src_void;
  char  *char_dst = (char*)dst_void + length;
  char  *char_src = (char*)src_void + length;
  size_t base_src, base_dst;
  size_t ii;

  /* NOTE: рассчитывает, что memcpy использует копирование
     от начала вперед */
  if (ba_src >= ba_dst)
    {
      memcpy(dst_void, src_void, length);
      RETURN (dst_void);
    }

  /* у нас 128-битные указатели */
  /* assert(sizeof(char*) > sizeof(long long)); */

  /* копируем задом наперед */

  /* копируем хвостик по байтам */
  DO_COPY(char, sizeof(int)-1);

  /* копируем остаток по словам */
  DO_COPY(int,  sizeof(long long)-1);

  /* для копирования квадро словами нужно так же чтобы базовый (не суммарный)
     адрес дескриптора был кратен восьми (честное слово). */
  base_src = __builtin_e2k_get_ap_base(src_void);
  base_dst = __builtin_e2k_get_ap_base(dst_void);

  if ((base_src & 7) == 0 && (base_dst & 7) == 0)
    {
      /* копируем остаток двойными словами */
      DO_COPY(long long, sizeof(char*)-1);

      /* то, что осталось, копируем указателями */
      length >>= 4;
      while (length--)
        {
	  char_dst -= sizeof(char*);
	  char_src -= sizeof(char*);
	  *((char**)char_dst) = *((char**)char_src);
        }
    }
  else
    {
      /* то, что осталось, копируем двойными словами */
      length >>= 3;
      while (length--)
        {
	  char_dst -= sizeof(long long);
	  char_src -= sizeof(long long);
	  *((long long*)char_dst) = *((long long*)char_src);
        }
    }

  RETURN (dst_void);
}

#ifndef memmove
libc_hidden_builtin_def (memmove)
#endif
