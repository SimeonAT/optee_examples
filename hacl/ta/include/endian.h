/*
 * Written by Claude (Anthropic, Claude Sonnet 5.5), November 20, 2026, at
 * the request of the repository author. To be reviewed and tested by the
 * author before use.
 *
 * Minimal <endian.h> for an OP-TEE 4.2.0 Trusted Application. OP-TEE's TA
 * libc does not provide <endian.h>, but KaRaMeL's lowstar_endianness.h
 * includes it whenever __linux__ is defined and expects these 12 macros.
 *
 * The definitions mirror the GCC-like branch KaRaMeL itself uses for
 * MinGW/Emscripten (hacl-star dist/karamel/include/krml/lowstar_endianness.h,
 * the _WIN32/__EMSCRIPTEN__ branch): __builtin_bswap* for the "be" side and
 * identity for the "le" side. That is only correct on a little-endian
 * target, so the #error below refuses to build on anything else.
 */
#ifndef TA_MINIMAL_ENDIAN_H
#define TA_MINIMAL_ENDIAN_H

#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "This minimal endian.h assumes a little-endian target (aarch64 LE)"
#endif

#define htobe16(x) __builtin_bswap16(x)
#define htole16(x) (x)
#define be16toh(x) __builtin_bswap16(x)
#define le16toh(x) (x)

#define htobe32(x) __builtin_bswap32(x)
#define htole32(x) (x)
#define be32toh(x) __builtin_bswap32(x)
#define le32toh(x) (x)

#define htobe64(x) __builtin_bswap64(x)
#define htole64(x) (x)
#define be64toh(x) __builtin_bswap64(x)
#define le64toh(x) (x)

#endif /* TA_MINIMAL_ENDIAN_H */