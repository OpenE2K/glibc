# Copyright (C) 1991-2019 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, see
# <http://www.gnu.org/licenses/>.

#
#	Makefile for setjmp/longjmp routines
#
subdir	:= setjmp

include ../Makeconfig

headers	:= setjmp.h bits/setjmp.h bits/setjmp2.h

routines	:= setjmp sigjmp bsd-setjmp bsd-_setjmp

ifeq ($(config-machine),e2k128)
# Adding routines to static-only-routines places them to libc_nonshared.a
# instead of libc.so and libc.a. See Bug #94897 for why this is crucial in
# Protected Mode.
static-only-routines	:= $(routines)
# However, because they are used within libc.so and ld.so internally, add them
# to shared-only-routines using different names. (In fact `shared-*' files just
# include their ordinary counterparts. TODO: find out if the same names may be
# present both in `{static,shared}-only-routines'.) Taking into account that
# all these functions are declared as hidden now, this shouldn't result in any
# conflicts.
shared-only-routines	:= shared-setjmp shared-sigjmp shared-bsd-_setjmp
# Functions from `{static,shared}-only-routines' lists should be specified in
# `routines' too!
routines		+= $(shared-only-routines)
endif

routines	+= longjmp __longjmp jmp-unwind

tests		:= tst-setjmp jmpbug bug269-setjmp tst-setjmp-fp \
		   tst-sigsetjmp tst-setjmp-static
tests-static	:= tst-setjmp-static


include ../Rules

$(objpfx)tst-setjmp-fp: $(libm)
