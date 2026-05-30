ifeq (yes,$(compile-with-lcc))
e2k-cflags = -fasm-entries-first -fno-strict-aliasing -fno-ipa-attrs
e2k-cflags += -D__OPTIMIZE__ -U__NO_INLINE__ -masm-inline

# See MCSTBug #60075 and MCSTBug #94443
e2k-cflags += -Wno-builtin-functions-redefined -Wno-builtin-declaration-mismatch

# These options are expected to suppress all currently known unreliable
# warnings produced by optimizing LCC making `--enable-werror' glibc build
# fail (MCSTBug #123898, Comment #0). They are attached unconditionally because
# as stated in Comment #7 and below a class of warnings emitted by LCC
# optimizer does not intersect with the one produced by EDG which is why
# EDG warnings are not going to be unreasonably inhibited.
e2k-cflags += -Wno-error=invalid-builtin-arg -Wno-error=maybe-uninitialized

gnulib-arch = -llcc
static-gnulib-arch = -llcc
ifeq ($(config-machine),e2k128)
# gnulib-arch += -latomic
static-gnulib-arch += -latomic
endif # e2k128
endif # compile-with-lcc

CC += $(e2k-cflags)
CXX += $(e2k-cflags)

# The E2k `long double' is a distinct type we support.
long-double-fcts = yes

ifeq ($(subdir),malloc)
ifeq ($(config-machine),e2k32)
# The default malloc () alignment in 32-bit mode is 8. Override it.
CFLAGS-malloc.c += -DMALLOC_ALIGNMENT=16
endif # e2k32
endif # Making in malloc/

ifeq ($(compile-with-lcc),yes)

ifneq (,$(filter -gwith-inline,$(CC)))

ifeq ($(subdir),csu)
CFLAGS-start.c += --true=spill_param_in_local_stack
endif # Making in csu/

ifeq ($(subdir),elf)
CFLAGS-dl-runtime.c += --true=spill_param_in_local_stack
endif # Making in elf/

endif # -gwith-inline

ifeq ($(extra-optimizations),yes)

ifeq ($(subdir),malloc)
# This magic combination of options was proposed in Bug #93150, Comment #12
CFLAGS-malloc.o = -O4 -faligned -finline-static-functions -fno-dam -fibranch-bound=0.02
endif # Making in malloc/

ifeq ($(subdir),stdlib)
CFLAGS-erand48_r.c += -frestrict-params
endif # Making in stdlib/

ifeq ($(subdir),string)
# Bug #137949: use special CFLAGS for a few files.
CFLAGS-memcpy.c += -faligned
CFLAGS-mempcpy.c += -faligned
CFLAGS-memmove.c += -faligned
CFLAGS-bcopy.c += -faligned
endif # Making in string/

endif # extra-optimizations

else # Not compile-with-lcc

ifeq ($(subdir),math)
CFLAGS-s_sinl.c += -fno-strict-aliasing
CFLAGS-s_sin.c += -fno-strict-aliasing
endif # Making in math/

ifeq ($(subdir),stdlib)
CFLAGS-erand48_r.c += -fno-strict-aliasing
CFLAGS-makecontext.c += -fno-strict-aliasing -Wno-stringop-overread -Wno-array-bounds
endif # Making in stdlib/

endif # Not compile-with-lcc
