e2k128-cflags =

ifeq (yes,$(compile-with-lcc))
e2k128-cflags += -Wno-void-pointer-to-function-pointer-cast

# Avoid allocating local VLA arrays via `__lcc_malloc ()' (this is
# the default behaviour in PM due to the inability of GETSAP to
# "return" the previously allocated stack) as the latter depends on
# `fprintf (stderr)' which is sure to break the linkage of ld.so
# (see MCSTBugs #109851 and #109796). This is done in hope that the use
# of GETSAPs to allocate VLAs in glibc won't result in stack
# overflow.
e2k128-cflags += -fstack-arrays
# See MCSTBug #135325, Comments #0 and #1.
e2k128-cflags += -Wno-int32-to-pointer-cast-protected

# Ensure that the addresses of structs' fields are evaluated in old-style
# way even when building glibc with compiler doing this in a more correct
# way (with MCSTBug #141345 fixed) to be on the safe side.
e2k128-cflags += -mprot-expanded-field-descriptor

endif # compile-with-lcc

CC += $(e2k128-cflags)
CXX += $(e2k128-cflags)
