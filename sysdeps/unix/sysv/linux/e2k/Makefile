ifeq ($(subdir),misc)
sysdep_routines += clone2
endif

ifeq ($(subdir),stdlib)
sysdep_routines += freecontext
endif

ifeq ($(subdir),csu)
extra-objs += rstart.o
CPPFLAGS-start.os += -DIN_START_OS
endif
