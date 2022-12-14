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

ifeq ($(subdir),gnulib)
sysdep_routines = dotmul umul $(divrem) alloca
endif	# gnulib

# We distribute these files, even though they are generated,
# so as to avoid the need for a functioning m4 to build the library.
divrem := sdiv udiv rem urem


ifeq ($(maintainer-mode),yes)

+divrem-NAME-sdiv := div
+divrem-NAME-udiv := udiv
+divrem-NAME-rem := rem
+divrem-NAME-urem := urem
+divrem-NAME = $(+divrem-NAME-$(basename $(notdir $@)))
+divrem-OP-div := div
+divrem-OP-udiv := div
+divrem-OP-rem := rem
+divrem-OP-urem := rem
+divrem-S-div := true
+divrem-S-rem := true
+divrem-S-udiv := false
+divrem-S-urem := false
$(divrem:%=$(sysdep_dir)/sparc/sparc32/%.S): $(sysdep_dir)/sparc/sparc32/divrem.m4
	(echo -n "define(NAME,\`.$(+divrem-NAME)')"; \
	 echo -n " define(OP,\`$(+divrem-OP-$(+divrem-NAME))')"; \
	 echo -n " define(S,\`$(+divrem-S-$(+divrem-NAME))')"; \
	 echo " /* This file is generated from divrem.m4; DO NOT EDIT! */"; \
	 cat $<) | $(M4) > $@-tmp
# Make it unwritable so noone will edit it by mistake.
	-chmod a-w $@-tmp
	mv -f $@-tmp $@

sysdep-realclean := $(sysdep-realclean) $(divrem:%=sysdeps/sparc/sparc32/%.S)

endif # maintainer-mode

# libgcc __divdi3 and __moddi3 uses .udiv and since it is also exported by
# libc.so linker will create PLTs for the symbol.  To avoid it we strong alias
# the exported libc one to __wrap_.udiv and use linker option --wrap to make any
# call to .udiv to call the wrapper symbol.
libc.so-gnulib += -Wl,--wrap=.udiv

ifeq ($(subdir),soft-fp)
sparc32-quad-routines := q_add q_cmp q_cmpe q_div q_dtoq q_feq q_fge	\
	q_fgt q_fle q_flt q_fne q_itoq q_mul q_neg q_qtod q_qtoi	\
	q_qtos q_qtou q_qtoull q_qtoll q_sqrt q_stoq q_sub q_utoq	\
	q_ulltoq q_lltoq q_util
sysdep_routines += $(sparc32-quad-routines)

endif
