/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Crypto Component                                                 */
/**                                                                       */
/**   Message Digest Algorithm - PSA/TF-M driver                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    nx_crypto_sha_psa.h                                 PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the NetX APIs used to perform hash cryptographic  */
/*    operations using PSA/TF-M crypto services.                          */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/



#ifndef SRC_NX_CRYPTO_SHA_PSA_H_
#define SRC_NX_CRYPTO_SHA_PSA_H_

/* Determine if a C++ compiler is being used.  If so, ensure that standard
   C is used to process the API information.  */
#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif


#include "nx_crypto.h"
#include "psa/crypto.h"

#define NX_CRYPTO_SHA2_BLOCK_SIZE_IN_BYTES  64
#define NX_CRYPTO_SHA224_ICV_LEN_IN_BITS    224
#define NX_CRYPTO_SHA256_ICV_LEN_IN_BITS    256

typedef struct NX_CRYPTO_SHA_PSA_STRUCT
{
    psa_algorithm_t nx_crypto_aes_psa_algorithm;
    psa_hash_operation_t nx_crypto_hash_psa_operation;
} NX_CRYPTO_SHA_PSA;


UINT _nx_crypto_sha_psa_initialize(NX_CRYPTO_SHA_PSA *context, UINT algorithm);
UINT _nx_crypto_sha_psa_update(NX_CRYPTO_SHA_PSA *context, UCHAR *input_ptr, UINT input_length);
UINT _nx_crypto_sha_psa_digest_calculate(NX_CRYPTO_SHA_PSA *context, UCHAR *digest, UINT algorithm);

UINT _nx_crypto_method_sha_psa_init(struct  NX_CRYPTO_METHOD_STRUCT *method,
                                   UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                   VOID  **handle,
                                   VOID  *crypto_metadata,
                                   ULONG crypto_metadata_size);

UINT _nx_crypto_method_sha_psa_cleanup(VOID *crypto_metadata);

UINT _nx_crypto_method_sha_psa_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
                                        VOID *handle, /* Crypto handler */
                                        struct NX_CRYPTO_METHOD_STRUCT *method,
                                        UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                        UCHAR *input, ULONG input_length_in_byte,
                                        UCHAR *iv_ptr,
                                        UCHAR *output, ULONG output_length_in_byte,
                                        VOID *crypto_metadata, ULONG crypto_metadata_size,
                                        VOID *packet_ptr,
                                        VOID (*nx_crypto_hw_process_callback)(VOID *packet_ptr, UINT status));

UINT nx_crypto_hash_clone_psa(VOID *dest_metadata, VOID *source_metadata, ULONG length);

#ifdef __cplusplus
}
#endif

#endif /* SRC_NX_CRYPTO_SHA_PSA_H_ */

