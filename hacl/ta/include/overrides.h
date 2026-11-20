/*
 * Written by Claude (Claude Sonnet 5.5, Anthropic).
 * Compiled for aarch64 with OP-TEE 4.2.0 TA dev kit [4].
 *
 * WHAT: overrides three libc calls that KaRaMeL makes in its error paths,
 *       using the KRML_HOST_* hooks documented in target.h [3].
 * WHY:  a TA has no stdio, exit() or time(), so the build fails with
 *       "implicit declaration of function 'time' / 'exit' / 'fprintf'".
 * HOW:  force-included into every file by `cflags-y += -include ...` in
 *       sub.mk, so these macros exist BEFORE target.h is read (target.h only
 *       defines its defaults when the macro is not already defined).
 *   printf/eprintf -> IMSG/EMSG: OP-TEE TA logging to the secure console [4].
 *   exit(code)     -> TEE_Panic(code): kills this TA instance [6]. KaRaMeL
 *                     only exits on "impossible" cases, so panicking fits.
 *   time()         -> constant 0: the algorithms we compile never call it.
 * DEBUG: a secure-log line "KaRaMeL incomplete match at <file>:<line>"
 *        followed by a TA panic means a HACL* function got an input outside
 *        its verified contract (for example a wrong buffer length).
 */
#ifndef OVERRIDES_H
#define OVERRIDES_H

#include <stdint.h>
#include <tee_api.h>   /* TEE_Panic */
#include <trace.h>     /* IMSG / EMSG */

#define KRML_HOST_PRINTF(...)   IMSG(__VA_ARGS__)
#define KRML_HOST_EPRINTF(...)  EMSG(__VA_ARGS__)
#define KRML_HOST_EXIT(code)    TEE_Panic((TEE_Result)(code))
#define KRML_HOST_TIME()        ((int32_t)0)

#endif /* OVERRIDES_H */