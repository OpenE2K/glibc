#include <string.h>
#include <memcopy.h>

#undef strcmp

/* Compare S1 and S2, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2.  */
int
strcmp (const char *s1,
	const char *s2)


{
  
  while (*s1 != '\0' && *s1 == *s2) 
  {
    s1++;
    s2++;
  }

  return (* (unsigned char *)s1) - (*(unsigned char *)s2);
}

libc_hidden_builtin_def (strcmp)
