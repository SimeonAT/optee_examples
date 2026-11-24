/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <string.h>

#include <Hacl_Ed25519.h>
#include <Hacl_HKDF.h>
#include <Lib_RandomBuffer_System.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* For the UUID (found in the TA's h-file(s)) */
#include <hacl_ta.h>

#include <common.h>

void ed25519_example() 
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_HELLO_WORLD_UUID;
	uint32_t err_origin;

	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
  }

	res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
  }

	memset(&op, 0, sizeof(op));

  // TODO: Use HKDF to create the Private Key.
  uint8_t private_key[ED_25519_PRIVATE_KEY_LENGTH];
  ClearBufMem(private_key, sizeof(private_key));
  // Lib_RandomBuffer_System_randombytes(private_key, sizeof(private_key));

  char *salt = "This is a test salt!";
  char *ikm = "This a test input key material!";
  char *ad = "This is test associated data!";

  uint8_t prk[SHA2_256_HASH_LENGTH];
  ClearBufMem(prk, sizeof(prk));

  Hacl_HKDF_extract_sha2_256(prk, (uint8_t *) salt, sizeof(salt), 
                             (uint8_t *) ikm, sizeof(ikm));
  Hacl_HKDF_expand_sha2_256(private_key, prk, sizeof(prk), (uint8_t *) ad, sizeof(ad),
                            ED_25519_PRIVATE_KEY_LENGTH);
  
  hexdump(private_key, sizeof(private_key), " --- Private Key ---");

  uint8_t public_key[ED_25519_PUBLIC_KEY_LENGTH];
  ClearBufMem(public_key, sizeof(public_key));
  Hacl_Ed25519_secret_to_public(public_key, private_key);

  uint8_t signature[ED_25519_SIGNATURE_LENGTH];
  ClearBufMem(signature, sizeof(signature));
  char* message = "Hello World!";

  Hacl_Ed25519_sign(signature, private_key, sizeof(message), (uint8_t *) message);
  hexdump(signature, sizeof(signature), "--- Signature ---");

	op.paramTypes = TEEC_PARAM_TYPES(
    TEEC_MEMREF_TEMP_INPUT,
    TEEC_MEMREF_TEMP_INPUT,
		TEEC_MEMREF_TEMP_INPUT,
    TEEC_NONE
  );

  op.params[0].tmpref.buffer = (void *) public_key;
  op.params[0].tmpref.size = ED_25519_PUBLIC_KEY_LENGTH;

  op.params[1].tmpref.buffer = (void *) message;
  op.params[1].tmpref.size = sizeof(message);

  op.params[2].tmpref.buffer = (void *) signature;
  op.params[2].tmpref.size = sizeof(signature);

	res = TEEC_InvokeCommand(&sess, TA_HELLO_WORLD_CMD_INC_VALUE, &op, &err_origin);
	if (res != TEEC_SUCCESS)
  {
    errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
  }

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);
  return;
}

int main(void)
{
  ed25519_example();
	return 0;
}
