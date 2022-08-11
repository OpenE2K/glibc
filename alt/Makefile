INSTALL = install
CFLAGS = $(RPM_OPT_FLAGS)
SRC = glibc_post_upgrade.c glibc_preinstall.c
C_TARGETS = $(SRC:.c=)
I_TARGETS = glibc_fix_post glibc_fix_nsswitch glibc_post_upgrade glibc_preinstall
L_TARGETS = post_ldconfig postun_ldconfig

.PHONY:	all install clean

all: $(C_TARGETS)

install: $(I_TARGETS)
	for f in $^; do \
		$(INSTALL) -pD -m700 $$f $(RPM_BUILD_ROOT)/sbin/$$f; \
	done
	for f in $(L_TARGETS); do \
		ln -snf ldconfig $(RPM_BUILD_ROOT)/sbin/$$f; \
	done

clean:
	$(RM) $(C_TARGETS) *~

GLIBC_PREINSTALL_CMD = $(LINK.o) -s -static -L$(path_link)
MULDEFS_FLAGS = -Wl,-zmuldefs

glibc_preinstall: glibc_preinstall.o
	if $(GLIBC_PREINSTALL_CMD) $< -o $@ 2>/dev/null; then \
		echo '$< unexpectedly built without $(MULDEFS_FLAGS)'; \
		exit 1; \
	fi
	$(GLIBC_PREINSTALL_CMD) $(MULDEFS_FLAGS) $< -o $@
