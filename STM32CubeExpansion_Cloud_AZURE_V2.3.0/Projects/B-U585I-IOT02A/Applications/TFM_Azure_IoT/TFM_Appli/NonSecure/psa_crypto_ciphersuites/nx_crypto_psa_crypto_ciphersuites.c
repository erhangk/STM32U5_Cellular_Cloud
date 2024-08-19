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
/**     Transport Layer Security (TLS)                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#include "nx_secure_tls.h"

#include "nx_crypto_ecdsa_psa_crypto.h"
#include "nx_crypto_aes_psa.h"
#include "nx_crypto_sha_psa.h"
#include "nx_crypto_rsa_psa.h"

//#include "nx_crypto_rsa.h"
#include "nx_crypto_aes.h"
#include "nx_crypto_sha1.h"
#include "nx_crypto_hmac.h"
#include "nx_crypto_sha2.h"
#include "nx_crypto_hmac_sha2.h"
#include "nx_crypto_md5.h"
#include "nx_crypto_hmac_md5.h"
#include "nx_crypto_sha5.h"
#include "nx_crypto_phash.h"
#include "nx_crypto_tls_prf_1.h"
#include "nx_crypto_tls_prf_sha256.h"
#include "nx_crypto_null.h"
#include "nx_crypto_ecdh.h"
#include "nx_crypto_ecdsa.h"
#include "nx_crypto_ecjpake.h"

/* Declare the ECDSA crypto method */
NX_CRYPTO_METHOD crypto_method_ecdsa_psa_crypto =
{
    NX_CRYPTO_DIGITAL_SIGNATURE_ECDSA,           /* ECDSA crypto algorithm                 */
    0,                                           /* Key size in bits                       */
    0,                                           /* IV size in bits                        */
    0,                                           /* ICV size in bits, not used             */
    0,                                           /* Block size in bytes                    */
    sizeof(NX_CRYPTO_ECDSA_PSA_CRYPTO),                 /* Metadata size in bytes                 */
    _nx_crypto_method_ecdsa_psa_crypto_init,            /* ECDSA initialization routine           */
    _nx_crypto_method_ecdsa_psa_crypto_cleanup,         /* ECDSA cleanup routine                  */
    _nx_crypto_method_ecdsa_psa_crypto_operation,       /* ECDSA operation                        */
};

/* Declare the AES-CBC 128 encryption method. */
NX_CRYPTO_METHOD crypto_method_aes_cbc_128_psa =
{
    NX_CRYPTO_ENCRYPTION_AES_CBC,                /* AES crypto algorithm                   */
    NX_CRYPTO_AES_128_KEY_LEN_IN_BITS,           /* Key size in bits                       */
    NX_CRYPTO_AES_IV_LEN_IN_BITS,                /* IV size in bits                        */
    0,                                           /* ICV size in bits, not used             */
    (NX_CRYPTO_AES_BLOCK_SIZE_IN_BITS >> 3),     /* Block size in bytes                    */
    sizeof(NX_CRYPTO_AES_PSA),                   /* Metadata size in bytes                 */
    _nx_crypto_method_aes_psa_init,              /* AES-CBC initialization routine         */
    _nx_crypto_method_aes_psa_cleanup,           /* AES-CBC cleanup routine                */
    _nx_crypto_method_aes_psa_operation          /* AES-CBC operation                      */
};

/* Declare the AES-CBC 192 encryption method. */
NX_CRYPTO_METHOD crypto_method_aes_cbc_192_psa =
{
    NX_CRYPTO_ENCRYPTION_AES_CBC,                /* AES crypto algorithm                   */
    NX_CRYPTO_AES_192_KEY_LEN_IN_BITS,           /* Key size in bits                       */
    NX_CRYPTO_AES_IV_LEN_IN_BITS,                /* IV size in bits                        */
    0,                                           /* ICV size in bits, not used             */
    (NX_CRYPTO_AES_BLOCK_SIZE_IN_BITS >> 3),     /* Block size in bytes                    */
    sizeof(NX_CRYPTO_AES_PSA),                   /* Metadata size in bytes                 */
    _nx_crypto_method_aes_psa_init,              /* AES-CBC initialization routine         */
    _nx_crypto_method_aes_psa_cleanup,           /* AES-CBC cleanup routine                */
    _nx_crypto_method_aes_psa_operation          /* AES-CBC operation                      */
};

/* Declare the AES-CBC 256 encryption method */
NX_CRYPTO_METHOD crypto_method_aes_cbc_256_psa =
{
    NX_CRYPTO_ENCRYPTION_AES_CBC,                /* AES crypto algorithm                   */
    NX_CRYPTO_AES_256_KEY_LEN_IN_BITS,           /* Key size in bits                       */
    NX_CRYPTO_AES_IV_LEN_IN_BITS,                /* IV size in bits                        */
    0,                                           /* ICV size in bits, not used             */
    (NX_CRYPTO_AES_BLOCK_SIZE_IN_BITS >> 3),     /* Block size in bytes                    */
    sizeof(NX_CRYPTO_AES_PSA),                   /* Metadata size in bytes                 */
    _nx_crypto_method_aes_psa_init,              /* AES-CBC initialization routine         */
    _nx_crypto_method_aes_psa_cleanup,           /* AES-CBC cleanup routine                */
    _nx_crypto_method_aes_psa_operation          /* AES-CBC operation                      */
};

/* Declare the SHA256 PSA hash method */
NX_CRYPTO_METHOD crypto_method_sha256_psa =
{
    NX_CRYPTO_HASH_SHA256,                         /* SHA256 algorithm                      */
    0,                                             /* Key size in bits                      */
    0,                                             /* IV size in bits, not used             */
    NX_CRYPTO_SHA256_ICV_LEN_IN_BITS,              /* Transmitted ICV size in bits          */
    NX_CRYPTO_SHA2_BLOCK_SIZE_IN_BYTES,            /* Block size in bytes                   */
    sizeof(NX_CRYPTO_SHA_PSA),                     /* Metadata size in bytes                */
    _nx_crypto_method_sha_psa_init,                /* SHA256 initialization routine         */
    _nx_crypto_method_sha_psa_cleanup,             /* SHA256 cleanup routine                */
    _nx_crypto_method_sha_psa_operation            /* SHA256 operation                      */
};

/* Declare the RSA public cipher method. */
NX_CRYPTO_METHOD crypto_method_rsa_psa =
{
    NX_CRYPTO_KEY_EXCHANGE_RSA,               /* RSA crypto algorithm                   */
    0,                                        /* Key size in bits                       */
    0,                                        /* IV size in bits                        */
    0,                                        /* ICV size in bits, not used.            */
    0,                                        /* Block size in bytes.                   */
    sizeof(NX_CRYPTO_RSA_PSA),                /* Metadata size in bytes                 */
    _nx_crypto_method_rsa_psa_init,           /* RSA initialization routine.            */
    _nx_crypto_method_rsa_psa_cleanup,        /* RSA cleanup routine                    */
    _nx_crypto_method_rsa_psa_operation       /* RSA operation                          */
};

#if 1
/* Define supported crypto method. */
extern NX_CRYPTO_METHOD crypto_method_hmac;
extern NX_CRYPTO_METHOD crypto_method_hmac_sha256;
extern NX_CRYPTO_METHOD crypto_method_tls_prf_sha256;
extern NX_CRYPTO_METHOD crypto_method_sha256;
extern NX_CRYPTO_METHOD crypto_method_aes_cbc_128;
extern NX_CRYPTO_METHOD crypto_method_aes_cbc_256;
extern NX_CRYPTO_METHOD crypto_method_rsa;
extern NX_CRYPTO_METHOD crypto_method_ecdhe;

extern NX_CRYPTO_METHOD crypto_method_tls_prf_sha_256;
extern NX_CRYPTO_METHOD crypto_method_null;
extern NX_CRYPTO_METHOD crypto_method_md5;
extern NX_CRYPTO_METHOD crypto_method_sha1;
extern NX_CRYPTO_METHOD crypto_method_sha384;
extern NX_CRYPTO_METHOD crypto_method_sha224;
extern NX_CRYPTO_METHOD crypto_method_sha512;
extern NX_CRYPTO_METHOD crypto_method_ecdsa;

extern NX_CRYPTO_METHOD crypto_method_ec_secp256;
extern NX_CRYPTO_METHOD crypto_method_ec_secp384;

/* Lookup table for X.509 digital certificates - they need a public-key algorithm and a hash routine for verification. */
NX_SECURE_X509_CRYPTO _nx_crypto_x509_cipher_lookup_table_ecc_psa_crypto[] =
{
    /* OID identifier,                        public cipher,            hash method */
    //{NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_256,  &crypto_method_ecdsa,     &crypto_method_sha256},
    //{NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_384,  &crypto_method_ecdsa,     &crypto_method_sha384},
    //{NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_512,  &crypto_method_ecdsa,     &crypto_method_sha512},

    {NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_256,  &crypto_method_ecdsa_psa_crypto,     &crypto_method_sha256},
    {NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_384,  &crypto_method_ecdsa_psa_crypto,     &crypto_method_sha384},
    {NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_512,  &crypto_method_ecdsa_psa_crypto,     &crypto_method_sha512},

    {NX_SECURE_TLS_X509_TYPE_RSA_SHA_256,    &crypto_method_rsa,       &crypto_method_sha256},
    {NX_SECURE_TLS_X509_TYPE_RSA_SHA_384,    &crypto_method_rsa,       &crypto_method_sha384},
    {NX_SECURE_TLS_X509_TYPE_RSA_SHA_512,    &crypto_method_rsa,       &crypto_method_sha512},
    {NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_224,  &crypto_method_ecdsa_psa_crypto,     &crypto_method_sha224},
    {NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_1,    &crypto_method_ecdsa_psa_crypto,     &crypto_method_sha1},
//    {NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_224,  &crypto_method_ecdsa,     &crypto_method_sha224},
//    {NX_SECURE_TLS_X509_TYPE_ECDSA_SHA_1,    &crypto_method_ecdsa,     &crypto_method_sha1},

    {NX_SECURE_TLS_X509_TYPE_RSA_SHA_1,      &crypto_method_rsa,       &crypto_method_sha1},
    {NX_SECURE_TLS_X509_TYPE_RSA_MD5,        &crypto_method_rsa,       &crypto_method_md5},
};

NX_SECURE_TLS_CIPHERSUITE_INFO _nx_crypto_ciphersuite_lookup_table_ecc_psa_crypto[] =
{
    /* Ciphersuite,                           public cipher,            public_auth,                     session cipher & cipher mode,   iv size, key size,  hash method,                   hash size,  TLS PRF */
#if (NX_SECURE_TLS_TLS_1_3_ENABLED)
    {TLS_AES_128_GCM_SHA256,                  &crypto_method_ecdhe,     &crypto_method_ecdsa_psa_crypto, &crypto_method_aes_128_gcm_16,  96,      16,        &crypto_method_sha256,         32,         &crypto_method_hkdf},
    {TLS_AES_128_CCM_SHA256,                  &crypto_method_ecdhe,     &crypto_method_ecdsa_psa_crypto, &crypto_method_aes_ccm_16,      96,      16,        &crypto_method_sha256,         32,         &crypto_method_hkdf},
    {TLS_AES_128_CCM_8_SHA256,                &crypto_method_ecdhe,     &crypto_method_ecdsa_psa_crypto, &crypto_method_aes_ccm_8,       96,      16,        &crypto_method_sha256,         32,         &crypto_method_hkdf},
#endif

#ifdef NX_SECURE_ENABLE_AEAD_CIPHER
    {TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256, &crypto_method_ecdhe,     &crypto_method_ecdsa_psa_crypto, &crypto_method_aes_128_gcm_16,  16,      16,        &crypto_method_null,            0,         &crypto_method_tls_prf_sha256},
    {TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,   &crypto_method_ecdhe,     &crypto_method_rsa,          &crypto_method_aes_128_gcm_16,  16,      16,        &crypto_method_null,            0,         &crypto_method_tls_prf_sha256},
#endif /* NX_SECURE_ENABLE_AEAD_CIPHER */

    {TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256, &crypto_method_ecdhe,     &crypto_method_ecdsa_psa_crypto, &crypto_method_aes_cbc_128_psa, 16,      16,        &crypto_method_hmac_sha256,     32,        &crypto_method_tls_prf_sha256},
    {TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,   &crypto_method_ecdhe,     &crypto_method_rsa,          &crypto_method_aes_cbc_128_psa, 16,      16,        &crypto_method_hmac_sha256,     32,        &crypto_method_tls_prf_sha256},

#ifdef NX_SECURE_ENABLE_AEAD_CIPHER
    {TLS_RSA_WITH_AES_128_GCM_SHA256,         &crypto_method_rsa,   &crypto_method_rsa,          &crypto_method_aes_128_gcm_16,  16,      16,        &crypto_method_null,            0,         &crypto_method_tls_prf_sha256},
#endif /* NX_SECURE_ENABLE_AEAD_CIPHER */

    {TLS_RSA_WITH_AES_256_CBC_SHA256,         &crypto_method_rsa,   &crypto_method_rsa,          &crypto_method_aes_cbc_256_psa, 16,      32,        &crypto_method_hmac_sha256,     32,        &crypto_method_tls_prf_sha256},
    {TLS_RSA_WITH_AES_128_CBC_SHA256,         &crypto_method_rsa,   &crypto_method_rsa,          &crypto_method_aes_cbc_128_psa, 16,      16,        &crypto_method_hmac_sha256,     32,        &crypto_method_tls_prf_sha256},

#ifdef NX_SECURE_ENABLE_PSK_CIPHERSUITES
    {TLS_PSK_WITH_AES_128_CBC_SHA256,         &crypto_method_null,      &crypto_method_auth_psk,         &crypto_method_aes_cbc_128_psa, 16,      16,        &crypto_method_hmac_sha256,     32,        &crypto_method_tls_prf_sha256},
#ifdef NX_SECURE_ENABLE_AEAD_CIPHER
    {TLS_PSK_WITH_AES_128_CCM_8,              &crypto_method_null,      &crypto_method_auth_psk,         &crypto_method_aes_ccm_8,       16,      16,        &crypto_method_null,            0,         &crypto_method_tls_prf_sha256},
#endif
#endif /* NX_SECURE_ENABLE_PSK_CIPHERSUITES */


};

//extern NX_SECURE_X509_CRYPTO *_nx_crypto_x509_cipher_lookup_table_ecc;
/* Define the object we can pass into TLS. */
const NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers_ecc_psa_crypto =
{
    /* Ciphersuite lookup table and size. */
    _nx_crypto_ciphersuite_lookup_table_ecc_psa_crypto,
    sizeof(_nx_crypto_ciphersuite_lookup_table_ecc_psa_crypto) / sizeof(NX_SECURE_TLS_CIPHERSUITE_INFO),

#ifndef NX_SECURE_DISABLE_X509
    /* X.509 certificate cipher table and size. */
    _nx_crypto_x509_cipher_lookup_table_ecc_psa_crypto,
    sizeof(_nx_crypto_x509_cipher_lookup_table_ecc_psa_crypto) / sizeof(NX_SECURE_X509_CRYPTO),
    //_nx_crypto_x509_cipher_lookup_table_ecc,
    //sizeof(_nx_crypto_x509_cipher_lookup_table_ecc) / sizeof(NX_SECURE_X509_CRYPTO),
#endif

    /* TLS version-specific methods. */
#if (NX_SECURE_TLS_TLS_1_0_ENABLED || NX_SECURE_TLS_TLS_1_1_ENABLED)
    &crypto_method_md5,
    &crypto_method_sha1,
    &crypto_method_tls_prf_1,
#endif

#if (NX_SECURE_TLS_TLS_1_2_ENABLED)
    &crypto_method_sha256,
    &crypto_method_tls_prf_sha256,
#endif

#if (NX_SECURE_TLS_TLS_1_3_ENABLED)
    &crypto_method_hkdf,
    &crypto_method_hmac,
    &crypto_method_ecdhe,
#endif

};

const USHORT nx_crypto_ecc_supported_groups_psa_crypto[] =
{
    (USHORT)NX_CRYPTO_EC_SECP256R1,
    (USHORT)NX_CRYPTO_EC_SECP384R1,
};

const NX_CRYPTO_METHOD *nx_crypto_ecc_curves_psa_crypto[] =
{
    &crypto_method_ec_secp256,
    &crypto_method_ec_secp384,
};

const UINT nx_crypto_ecc_supported_groups_size_psa_crypto = sizeof(nx_crypto_ecc_supported_groups_psa_crypto) / sizeof(USHORT);
#endif
