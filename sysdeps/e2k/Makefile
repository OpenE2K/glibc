ifeq (yes,$(compile-with-lcc))
gnulib-arch = -llcc
static-gnulib-arch = -llcc
ifeq ($(config-machine),e2k128)
# gnulib-arch += -latomic
static-gnulib-arch += -latomic
endif # e2k128
endif # compile-with-lcc

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
