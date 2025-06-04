#ifndef _ARCH_UNWIND_LINK_H
#define _ARCH_UNWIND_LINK_H

#define UNWIND_LINK_GETIP 1
#define UNWIND_LINK_FRAME_STATE_FOR 1
#define UNWIND_LINK_FRAME_ADJUSTMENT 0


#define UNWIND_LINK_EXTRA_FIELDS \
  __typeof (_Unwind_GetPCSP) *ptr__Unwind_GetPCSP;

#define UNWIND_LINK_EXTRA_INIT				      \
  local.ptr__Unwind_GetPCSP                                   \
  = __libc_dlsym (local_libgcc_handle, "_Unwind_GetPCSP");    \
  assert (local.ptr__Unwind_GetPCSP != NULL);                 \
  PTR_MANGLE (local.ptr__Unwind_GetPCSP);

#endif /* _ARCH_UNWIND_LINK_H */
