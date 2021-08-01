ifeq ($(subdir),misc)
sysdep_headers += e2k128/futex.h
sysdep_routines += futex
endif

# This was a no longer relevant attempt to ensure that setjmp is linked into
# ld.so after it had been made a member of `$(static-only-routines)'. This
# didn't help since it had to be linked into libc.so too.

# ifeq ($(subdir),elf)
# sysdep-rtld-routines += setjmp
# endif

ifeq ($(subdir),csu)

# FIXME: the below setting and rule have stupidly been borrowed from mach.
# However, won't `csu/Makefile' take care of that in some regular way in
# the presence of $(static-start-installed-name) (provided by . . ./e2k128/
# Makeconfig) differing from $(start-installed-name)?

# extra-objs += static-start.o

# `csu/Makefile' takes care of `g$(static-start-installed-name)' rule only
# in fact, which is something incomprehensible to me!
$(objpfx)$(static-start-installed-name): $(objpfx)static-start.o $(objpfx)abi-note.o \
					 $(objpfx)init.o $(objpfx)static-reloc.o
	$(link-relocatable)

endif
