#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined HAVE_STRING_H || defined _LIBC
# include <string.h>
#endif

#undef memcmp

#ifndef MEMCMP
# define MEMCMP memcmp
#endif

#ifdef _LIBC

# include <memcopy.h>
# include <endian.h>

# if __BYTE_ORDER == __BIG_ENDIAN
#  define WORDS_BIGENDIAN
# endif

#else	/* Not in the GNU C library.  */

# include <sys/types.h>

/* Type to use for aligned memory operations.
   This should normally be the biggest type supported by a single load
   and store.  Must be an unsigned type.  */
# define op_t	unsigned long int
# define OPSIZ	(sizeof(op_t))

/* Threshold value for when to enter the unrolled loops.  */
# define OP_T_THRES	16

/* Type to use for unaligned operations.  */
typedef unsigned char byte;

# ifndef WORDS_BIGENDIAN
#  define MERGE(w0, sh_1, w1, sh_2) (((w0) >> (sh_1)) | ((w1) << (sh_2)))
# else
#  define MERGE(w0, sh_1, w1, sh_2) (((w0) << (sh_1)) | ((w1) >> (sh_2)))
# endif

#endif	/* In the GNU C library.  */

int
MEMCMP (const void *s1, const void *s2, size_t len)
{
  unsigned char *c1 = (unsigned char *) s1;
  unsigned char *c2 = (unsigned char *) s2;

  while (len--)
    {
      if (*c1 != *c2) 
	return *c1 - *c2;

    c1++;
    c2++;
  }
  return 0;

}

libc_hidden_builtin_def(memcmp)
#ifdef weak_alias
# undef bcmp
weak_alias (memcmp, bcmp)
#endif
