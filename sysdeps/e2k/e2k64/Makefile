e2k64-cflags =

CC += $(e2k64-cflags)
CXX += $(e2k64-cflags)

ifeq ($(subdir),elf)

before-compile += $(objpfx)dl-machine.h
generated += dl-machine.h

$(objpfx)dl-machine.h: $(..)sysdeps/e2k/dl-machine.h
	$(make-target-directory)
	sed -e "s#XX#64#g" <$< >$@
endif
