/*
 * Written by Claude (Claude Sonnet 5.5, Anthropic). 
 * Compiled for aarch64 with OP-TEE 4.2.0 TA dev kit [4].
 *
 * WHAT: a minimal <endian.h> for an OP-TEE Trusted Application.
 * WHY:  KaRaMeL's lowstar_endianness.h does `#include <endian.h>` whenever
 *       __linux__ is defined. The TA's C library has no <endian.h>, so
 *       the build stops with "fatal error: endian.h: No such file".
 * HOW:  defines the 12 conversion macros KaRaMeL expects. "to little-endian"
 *       is the identity (aarch64 is little-endian here); "to big-endian" is a
 *       byte swap via the GCC builtins. This mirrors the GCC branch of
 *       KaRaMeL's own fallback in lowstar_endianness.h.
 * DEBUG: if you see "assumes a little-endian target", you are not compiling
 *        for little-endian aarch64; this shim is then WRONG, do not remove
 *        the #error.
 */
#ifndef ENDIAN_H
#define ENDIAN_H

/* Refuse to build on big-endian: identity macros below would be incorrect. */
#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "This minimal endian.h assumes a little-endian target (aarch64 LE)"
#endif

/* host -> big-endian / big-endian -> host: swap bytes (little-endian host) */
#define htobe16(x) __builtin_bswap16(x)
#define be16toh(x) __builtin_bswap16(x)
#define htobe32(x) __builtin_bswap32(x)
#define be32toh(x) __builtin_bswap32(x)
#define htobe64(x) __builtin_bswap64(x)
#define be64toh(x) __builtin_bswap64(x)

/* host <-> little-endian: already the same byte order, so identity */
#define htole16(x) (x)
#define le16toh(x) (x)
#define htole32(x) (x)
#define le32toh(x) (x)
#define htole64(x) (x)
#define le64toh(x) (x)

#endif /* ENDIAN_H */