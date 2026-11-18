# Modified with the assistance of Claude (Anthropic AI assistant), 2026-11-18:
# added the HACL* build and link rules (include paths, libevercrypt.a built
# from hacl-star/dist/gcc-compatible using the ARM configs in
# hacl-star-configs-arm, and the $(OBJS) link line). Reviewed and tested by
# the repository author.
#
CC      ?= $(CROSS_COMPILE)gcc
LD      ?= $(CROSS_COMPILE)ld
AR      ?= $(CROSS_COMPILE)ar
NM      ?= $(CROSS_COMPILE)nm
OBJCOPY ?= $(CROSS_COMPILE)objcopy
OBJDUMP ?= $(CROSS_COMPILE)objdump
READELF ?= $(CROSS_COMPILE)readelf

OBJS = main.o

# --- HACL* (built from source for the same target as this binary) ---
HACL_DIST    = ../../hacl-star/dist
HACL_GCC     = $(HACL_DIST)/gcc-compatible
HACL_CONFIGS = ../../hacl-star-configs-arm
HACL_LIB     = $(HACL_GCC)/libevercrypt.a

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT)/include -I./include
CFLAGS += -I$(HACL_GCC) -I$(HACL_DIST)/karamel/include \
          -I$(HACL_DIST)/karamel/krmllib/dist/minimal
#Add/link other required libraries here
LDADD += $(HACL_LIB) -lteec -L$(TEEC_EXPORT)/lib

BINARY = optee_example_hacl_ed25519

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS) $(HACL_LIB)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDADD)

# HACL*'s Makefile runs its native ./configure if Makefile.config is missing,
# so put the ARM configs in place first, then build only the static library.
$(HACL_GCC)/Makefile.config: $(HACL_CONFIGS)/Makefile.config
	cp $< $@
$(HACL_GCC)/config.h: $(HACL_CONFIGS)/config.h
	cp $< $@

# env -u MAKEFLAGS: the parent passes --no-builtin-variables, which breaks
# HACL*'s Makefile (it relies on make's built-in compile rule).
$(HACL_LIB): $(HACL_GCC)/Makefile.config $(HACL_GCC)/config.h
	env -u MAKEFLAGS -u MFLAGS $(MAKE) -j8 -C $(HACL_GCC) CC="$(CC)" AR="$(AR)" libevercrypt.a

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY)

.PHONY: hacl-clean
hacl-clean:
	$(MAKE) -C $(HACL_GCC) clean
	rm -f $(HACL_GCC)/libevercrypt.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@