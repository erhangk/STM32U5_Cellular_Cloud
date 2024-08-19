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
/**   Elliptic Curve Digital Signature Algorithm (ECDSA)                  */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    NX_CRYPTO_ECDSA_PSA_CRYPTO.h                        PORTABLE C     */
/*                                                           6.x          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the basic Application Interface (API) to the      */
/*    NetX ECDSA module.                                                  */
/*                                                                        */
/*    It is assumed that nx_api.h and nx_port.h have already been         */
/*    included.                                                           */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/

#ifndef  NX_CRYPTO_ECDSA_PSA_CRYPTO_H
#define  NX_CRYPTO_ECDSA_PSA_CRYPTO_H

/* Determine if a C++ compiler is being used.  If so, ensure that standard
   C is used to process the API information.  */
#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif

#include "nx_crypto.h"
#include "nx_crypto_ec.h"
#include <stdint.h>
#include "psa/crypto.h"

#ifndef NX_CRYPTO_ECDSA_PSA_CRYPTO_MAX_KEY_SIZE
#define NX_CRYPTO_ECDSA_PSA_CRYPTO_MAX_KEY_SIZE     (48)
#endif /* NX_CRYPTO_ECDSA_PSA_CRYPTO_MAX_KEY_SIZE */



/* ECDSA signature structure. */
typedef struct NX_CRYPTO_ECDSA_PSA_CRYPTO
{
    NX_CRYPTO_EC *nx_crypto_ecdsa_curve;
    NX_CRYPTO_METHOD *nx_crypto_ecdsa_hash_method;
    uint32_t key_id;
    psa_algorithm_t alg;

    UCHAR nx_crypto_ecdsa_scratch[2 * NX_CRYPTO_ECDSA_PSA_CRYPTO_MAX_KEY_SIZE];

/*
    UCHAR nx_crypto_ecdsa_hash[STSAFEA_SIGN_ECDSA_SHA384_LENGTH];
    UINT nx_crypto_ecdsa_hash_length;
    UCHAR nx_crypto_ecdsa_signature[STSAFEA_SIGN_ECDSA_SHA384_LENGTH];
    UINT nx_crypto_ecdsa_signature_length;
*/
} NX_CRYPTO_ECDSA_PSA_CRYPTO;

/* Define the function prototypes for ECDSA.  */

UINT _nx_crypto_ecdsa_sign_psa_crypto(NX_CRYPTO_ECDSA_PSA_CRYPTO *ecdsa_ptr,
                               UCHAR *hash,
                               UINT hash_length,
                               UCHAR *private_key,
                               UINT private_key_length,
                               UCHAR *signature,
                               UINT signature_length,
                               UINT *actual_signature_length);

UINT _nx_crypto_ecdsa_verify_psa_crypto(NX_CRYPTO_ECDSA_PSA_CRYPTO *ecdsa_ptr,
                                 UCHAR *hash,
                                 UINT hash_length,
                                 UCHAR *public_key,
                                 UINT public_key_length,
                                 UCHAR *signature,
                                 UINT signature_length);

UINT _nx_crypto_method_ecdsa_psa_crypto_init(struct  NX_CRYPTO_METHOD_STRUCT *method,
                                      UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                      VOID  **handle,
                                      VOID  *crypto_metadata,
                                      ULONG crypto_metadata_size);

UINT _nx_crypto_method_ecdsa_psa_crypto_cleanup(VOID *crypto_metadata);

UINT _nx_crypto_method_ecdsa_psa_crypto_operation(UINT op,
                                           VOID *handle,
                                           struct NX_CRYPTO_METHOD_STRUCT *method,
                                           UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                           UCHAR *input, ULONG input_length_in_byte,
                                           UCHAR *iv_ptr,
                                           UCHAR *output, ULONG output_length_in_byte,
                                           VOID *crypto_metadata, ULONG crypto_metadata_size,
                                           VOID *packet_ptr,
                                           VOID (*nx_crypto_hw_process_callback)(VOID *, UINT));

#ifdef __cplusplus
}
#endif

#endif /* NX_CRYPTO_ECDSA_PSA_CRYPTO_H */
