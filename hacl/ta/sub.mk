# Modified with the assistance of Claude (Anthropic, Claude Sonnet 5.5),
# November 2026: added the full HACL* source list and include paths (ARM
# config, minus Lib_PrintBuffer.c and Lib_RandomBuffer_System.c). To be
# reviewed and tested by the repository author.

# Absolute paths, derived from this file's own location (sub.mk is
# `include`d directly by mk/subdir.mk, so MAKEFILE_LIST is valid here).
HACL_TA_DIR  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
HACL_DIST    := $(abspath $(HACL_TA_DIR)../../hacl-star/dist)
HACL_GCC     := $(HACL_DIST)/gcc-compatible
HACL_CONFIGS := $(abspath $(HACL_TA_DIR)../../hacl-star-configs-arm)

global-incdirs-y += include

# incdirs_ext-y is used as written; global-incdirs-y gets the sub-dir
# prefixed onto each entry, which would mangle absolute paths.
incdirs_ext-y += $(HACL_GCC)
incdirs_ext-y += $(HACL_DIST)/karamel/include
incdirs_ext-y += $(HACL_DIST)/karamel/krmllib/dist/minimal
incdirs_ext-y += $(HACL_CONFIGS)

srcs-y += hello_world_ta.c

# Object list = the `ar cr libevercrypt.a ...` line printed by HACL*'s make
# for the ARM config, minus Lib_PrintBuffer and Lib_RandomBuffer_System.
HACL_SRCS := \
  EverCrypt_AEAD EverCrypt_AutoConfig2 EverCrypt_Chacha20Poly1305 \
  EverCrypt_Cipher EverCrypt_Curve25519 EverCrypt_DRBG EverCrypt_Ed25519 \
  EverCrypt_HKDF EverCrypt_HMAC EverCrypt_Hash EverCrypt_Poly1305 \
  Hacl_AEAD_Chacha20Poly1305 Hacl_AEAD_Chacha20Poly1305_Simd128 \
  Hacl_Bignum Hacl_Bignum256 Hacl_Bignum256_32 Hacl_Bignum32 \
  Hacl_Bignum4096 Hacl_Bignum4096_32 Hacl_Bignum64 \
  Hacl_Chacha20 Hacl_Chacha20_Vec128 Hacl_Chacha20_Vec32 \
  Hacl_Curve25519_51 Hacl_EC_Ed25519 Hacl_EC_K256 Hacl_Ed25519 Hacl_FFDHE \
  Hacl_Frodo1344 Hacl_Frodo64 Hacl_Frodo640 Hacl_Frodo976 Hacl_Frodo_KEM \
  Hacl_GenericField32 Hacl_GenericField64 \
  Hacl_HKDF Hacl_HKDF_Blake2s_128 Hacl_HMAC Hacl_HMAC_Blake2s_128 \
  Hacl_HMAC_DRBG \
  Hacl_HPKE_Curve51_CP128_SHA256 Hacl_HPKE_Curve51_CP128_SHA512 \
  Hacl_HPKE_Curve51_CP32_SHA256 Hacl_HPKE_Curve51_CP32_SHA512 \
  Hacl_HPKE_P256_CP128_SHA256 Hacl_HPKE_P256_CP32_SHA256 \
  Hacl_Hash_Base Hacl_Hash_Blake2b Hacl_Hash_Blake2s \
  Hacl_Hash_Blake2s_Simd128 Hacl_Hash_MD5 Hacl_Hash_SHA1 Hacl_Hash_SHA2 \
  Hacl_Hash_SHA3 Hacl_K256_ECDSA Hacl_MAC_Poly1305 \
  Hacl_MAC_Poly1305_Simd128 Hacl_NaCl Hacl_P256 Hacl_RSAPSS \
  Hacl_SHA2_Vec128 Hacl_Salsa20 Hacl_Streaming_HMAC \
  Vale Lib_Memzero0

srcs-y += $(addprefix $(HACL_GCC)/,$(addsuffix .c,$(HACL_SRCS)))

# HACL*'s own Makefile gives exactly these files -march=armv8-a+simd.
# cflags-<entry>-y is keyed by the exact srcs-y string (the absolute path).
# -mgeneral-regs-only (added to TA cflags when hard-float is off) would
# forbid NEON intrinsics, so it is removed for these files only.
HACL_SIMD128 := \
  Hacl_MAC_Poly1305_Simd128 Hacl_Chacha20_Vec128 \
  Hacl_AEAD_Chacha20Poly1305_Simd128 Hacl_Hash_Blake2s_Simd128 \
  Hacl_HMAC_Blake2s_128 Hacl_HKDF_Blake2s_128 Hacl_SHA2_Vec128

$(foreach f,$(HACL_SIMD128),$(eval cflags-$(HACL_GCC)/$(f).c-y += -march=armv8-a+simd))
$(foreach f,$(HACL_SIMD128),$(eval cflags-remove-$(HACL_GCC)/$(f).c-y += -mgeneral-regs-only))

# TODO: Lib_RandomBuffer_System_randombytes (bool, uint8_t *, uint32_t),
# backed by TEE_GenerateRandom. Create the file, then uncomment.
#srcs-y += hacl_random_ta.c

# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes