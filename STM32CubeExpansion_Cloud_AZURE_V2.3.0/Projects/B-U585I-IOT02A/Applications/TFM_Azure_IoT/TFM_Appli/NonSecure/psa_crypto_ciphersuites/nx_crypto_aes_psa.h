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
/**   AES Encryption - PSA/TF-M driver                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  APPLICATION INTERFACE DEFINITION                       RELEASE        */
/*                                                                        */
/*    nx_crypto_aes_psa_crypto.h                          PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the NetX APIs used to perform AES cryptographic   */
/*    operations using PSA/TF-M crypto services.                          */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/

#ifndef NX_CRYPTO_AES_PSA_H
#define NX_CRYPTO_AES_PSA_H

/* Determine if a C++ compiler is being used.  If so, ensure that standard
   C is used to process the API information.  */
#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif

/* Include the ThreadX and port-specific data type file.  */

#include "nx_crypto.h"
#include "psa/crypto.h"

/* AES expects key sizes in the number of 32-bit words each key takes. */
#define NX_CRYPTO_AES_KEY_SIZE_128_BITS          (4)
#define NX_CRYPTO_AES_KEY_SIZE_192_BITS          (6)
#define NX_CRYPTO_AES_KEY_SIZE_256_BITS          (8)

#define NX_CRYPTO_AES_256_KEY_LEN_IN_BITS        (256)
#define NX_CRYPTO_AES_192_KEY_LEN_IN_BITS        (192)
#define NX_CRYPTO_AES_128_KEY_LEN_IN_BITS        (128)
#define NX_CRYPTO_AES_XCBC_MAC_KEY_LEN_IN_BITS   (128)

#define NX_CRYPTO_AES_MAX_KEY_SIZE               (NX_CRYPTO_AES_KEY_SIZE_256_BITS) /* Maximum key size in bytes. */

#define NX_CRYPTO_AES_BLOCK_SIZE                 (16)                              /* The AES block size for all NetX Crypto operations, in bytes. */
#define NX_CRYPTO_AES_BLOCK_SIZE_IN_BITS         (128)
#define NX_CRYPTO_AES_IV_LEN_IN_BITS             (128)
#define NX_CRYPTO_AES_CTR_IV_LEN_IN_BITS         (64)

#define NX_CRYPTO_AES_KEY_SCHEDULE_UNKNOWN       0
#define NX_CRYPTO_AES_KEY_SCHEDULE_ENCRYPT       1
#define NX_CRYPTO_AES_KEY_SCHEDULE_DECRYPT       2


typedef struct NX_CRYPTO_AES_PSA_STRUCT
{
    /* Number of *words* in the cipher key - can be 4 (128 bits), 6 (192 bits), or 8 (256 bits). */
    USHORT nx_crypto_aes_key_size;

    /* PSA key handle that represents the key when calling into TF-M. */
    psa_key_handle_t nx_crypto_aes_key_handle;

    /* PSA algorithm identifier used to determine algorithm mode and
       type (e.g. AES-CBC). */
    psa_algorithm_t nx_crypto_aes_psa_algorithm;

    /* PSA operation - PSA control structure to use for an AES operation. */
    psa_cipher_operation_t nx_crypto_aes_psa_operation;

    /* Initialization flag for PSA cipher operation - if there is a way to determine
       whether a psa_cipher_operation_t structure is in use this flag can be removed. */
    USHORT nx_crypto_aes_psa_initialized;


} NX_CRYPTO_AES_PSA;

UINT _nx_crypto_aes_psa_encrypt(NX_CRYPTO_AES_PSA *aes_ptr, UCHAR *input, UCHAR *output, UINT length);
UINT _nx_crypto_aes_psa_decrypt(NX_CRYPTO_AES_PSA *aes_ptr, UCHAR *input, UCHAR *output, UINT length);

UINT _nx_crypto_aes_psa_key_set(NX_CRYPTO_AES_PSA *aes_ptr, UCHAR *key, UINT key_size);

UINT _nx_crypto_method_aes_psa_init(struct NX_CRYPTO_METHOD_STRUCT *method,
                                UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                VOID **handle,
                                VOID *crypto_metadata,
                                ULONG crypto_metadata_size);

UINT _nx_crypto_method_aes_psa_cleanup(VOID *crypto_metadata);

UINT _nx_crypto_method_aes_psa_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
                                     VOID *handle, /* Crypto handler */
                                     struct NX_CRYPTO_METHOD_STRUCT *method,
                                     UCHAR *key,
                                     NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                     UCHAR *input,
                                     ULONG input_length_in_byte,
                                     UCHAR *iv_ptr,
                                     UCHAR *output,
                                     ULONG output_length_in_byte,
                                     VOID *crypto_metadata,
                                     ULONG crypto_metadata_size,
                                     VOID *packet_ptr,
                                     VOID (*nx_crypto_hw_process_callback)(VOID *packet_ptr, UINT status));

UINT  _nx_crypto_method_aes_psa_cbc_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
                                          VOID *handle, /* Crypto handler */
                                          struct NX_CRYPTO_METHOD_STRUCT *method,
                                          UCHAR *key,
                                          NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                          UCHAR *input,
                                          ULONG input_length_in_byte,
                                          UCHAR *iv_ptr,
                                          UCHAR *output,
                                          ULONG output_length_in_byte,
                                          VOID *crypto_metadata,
                                          ULONG crypto_metadata_size,
                                          VOID *packet_ptr,
                                          VOID (*nx_crypto_hw_process_callback)(VOID *packet_ptr, UINT status));

NX_CRYPTO_KEEP UINT  _nx_crypto_method_aes_psa_gcm_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
                                                         VOID *handle, /* Crypto handler */
                                                         struct NX_CRYPTO_METHOD_STRUCT *method,
                                                         UCHAR *key,
                                                         NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                         UCHAR *input,
                                                         ULONG input_length_in_byte,
                                                         UCHAR *iv_ptr,
                                                         UCHAR *output,
                                                         ULONG output_length_in_byte,
                                                         VOID *crypto_metadata,
                                                         ULONG crypto_metadata_size,
                                                         VOID *packet_ptr,
                                                         VOID (*nx_crypto_hw_process_callback)(VOID *packet_ptr, UINT status));


#ifdef __cplusplus
}
#endif


#endif /* NX_CRYPTO_AES_PSA_H */

