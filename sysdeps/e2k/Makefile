ifeq (yes,$(compile-with-lcc))
gnulib-arch = -llcc
static-gnulib-arch = -llcc
ifeq ($(config-machine),e2k128)
# gnulib-arch += -latomic
static-gnulib-arch += -latomic
endif # e2k128
endif

# The E2k `long double' is a distinct type we support.
long-double-fcts = yes

# The default malloc () alignment in 32-bit mode is 8. Override it.
# FIXME: there is no point in doing so in 64-bit mode.
ifeq ($(subdir),malloc)
CFLAGS-malloc.c += -DMALLOC_ALIGNMENT=16
endif

ifeq ($(compile-with-lcc),yes)
ifneq (,$(filter -gwith-inline,$(CC)))

ifeq ($(subdir),csu)
CFLAGS-start.c += --true=spill_param_in_local_stack
endif

ifeq ($(subdir),elf)
CFLAGS-dl-runtime.c += --true=spill_param_in_local_stack
endif

endif
endif
