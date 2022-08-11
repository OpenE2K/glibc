#ifndef _INTERNAL_TYPES_H_SPARC_V8_
#define _INTERNAL_TYPES_H_SPARC_V8_ 1

#include <sysdeps/nptl/internaltypes.h>

/* The old bogus implementation of barrier data structure used prior to commit
   b02840bacdefde318d2ad2f920e50785b9b25d69.  */
struct pthread_barrier
{
  unsigned int curr_event;
  int lock;
  unsigned int left;
  unsigned int init_count;
  int private;
};


#endif /* _INTERNAL_TYPES_H_SPARC_V8_  */
