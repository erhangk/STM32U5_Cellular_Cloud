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
/**   RSA encryption - PSA/TF-M Driver                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    nx_crypto_rsa_psa.h                                  PORTABLE C     */
/*                                                           6.x          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the basic Application Interface (API) to the      */
/*    NetX RSA module for PSA/TF-M platforms.                             */
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

#ifndef  NX_CRYPTO_RSA_PSA_H
#define  NX_CRYPTO_RSA_PSA_H

/* Determine if a C++ compiler is being used.  If so, ensure that standard
   C is used to process the API information.  */
#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif


/* Include the ThreadX and port-specific data type file.  */

#include "nx_crypto.h"
#include "psa/crypto.h"

/* Define the maximum size of an RSA modulus supported in bits. */
#ifndef NX_CRYPTO_MAX_RSA_MODULUS_SIZE
#define NX_CRYPTO_MAX_RSA_MODULUS_SIZE    (4096) /* Default is to support 4096-bit RSA keys. */
#endif

/* Control block for RSA cryptographic operations. */
typedef struct NX_CRYPTO_RSA_PSA_STRUCT
{
    /* Pointer to hash method for signature verification. */
    NX_CRYPTO_METHOD *nx_crypto_rsa_psa_hash_method;

    /* Handle to PSA/TF-M key. */
    psa_key_handle_t nx_crypto_rsa_psa_key_handle;

    /* Algorithm being used with TF-M. */
    psa_algorithm_t nx_crypto_rsa_psa_alg;

    /* PSA operation - PSA control structure to use for an RSA operation. */
    psa_cipher_operation_t nx_crypto_rsa_psa_operation;


} NX_CRYPTO_RSA_PSA;


/* Function prototypes */


UINT _nx_crypto_method_rsa_psa_cleanup(VOID *crypto_metadata);

UINT _nx_crypto_method_rsa_psa_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
                                     VOID *handle, /* Crypto handler */
                                     struct NX_CRYPTO_METHOD_STRUCT *method,
                                     UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                     UCHAR *input, ULONG input_length_in_byte,
                                     UCHAR *iv_ptr,
                                     UCHAR *output, ULONG output_length_in_byte,
                                     VOID *crypto_metadata, ULONG crypto_metadata_size,
                                     VOID *packet_ptr,
                                     VOID (*nx_crypto_hw_process_callback)(VOID *packet_ptr, UINT status));

UINT _nx_crypto_method_rsa_psa_init(struct NX_CRYPTO_METHOD_STRUCT *method,
                                UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                VOID **handle,
                                VOID *crypto_metadata, ULONG crypto_metadata_size);
#ifdef __cplusplus
}
#endif

#endif /* NX_CRYPTO_ECDSA_PSA_CRYPTO_H */
