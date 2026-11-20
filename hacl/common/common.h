#pragma once

#include <Hacl_Ed25519.h>
#include <Lib_RandomBuffer_System.h>

#define ED_25519_PRIVATE_KEY_LENGTH 32
#define ED_25519_PUBLIC_KEY_LENGTH 32
#define ED_25519_SIGNATURE_LENGTH 64

#define ClearBufMem(buffer, size) memset(buffer, 0, size)