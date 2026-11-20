#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <Lib_RandomBuffer_System.h>

/**
 * WARNING: This function is deprecated.
 * https://hacl-star.github.io/Randomness.html?highlight=lib_randombuffer_system_randombytes
 * 
 * HACL* does not specify an implementation for its randomness function in OP-TEE.
 * Thus, this function defines an implementation to enable use of the HACL* randomness function
 * in OP-TEE.
 */
bool Lib_RandomBuffer_System_randombytes(uint8_t *x, uint32_t len)
{
  TEE_GenerateRandom(x, len);
  return true;
}