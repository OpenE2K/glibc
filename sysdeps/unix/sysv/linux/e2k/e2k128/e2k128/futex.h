#ifndef __E2K128_FUTEX_H
#define __E2K128_FUTEX_H

#include <features.h>

__BEGIN_DECLS

extern int futex (void *uaddr, int futex_op, int val, ...);

__END_DECLS

#endif /* __E2K128_FUTEX_H  */
