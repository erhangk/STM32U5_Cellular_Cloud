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

#include "nx_crypto_rsa_psa.h"

UINT _nx_crypto_rsa_psa_key_import(NX_CRYPTO_RSA_PSA *rsa_ptr, psa_algorithm_t alg, psa_key_type_t key_type,
                                                uint8_t *private_key, size_t private_key_length)
{
psa_key_handle_t key_handle;
psa_status_t status;

#if (TFM_1_0 == 1)
psa_key_policy_t policy = psa_key_policy_init();
#elif (TFM_1_3 == 1)
psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
#else
#error "Must define a TF-M version"
#endif

psa_key_usage_t usage = (PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY);

#if (TFM_1_0 == 1)
    /* Allocate a transient key */
    status = psa_allocate_key(&key_handle);
    if (status != PSA_SUCCESS) {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    /* Setup the key policy */
    psa_key_policy_set_usage(&policy, usage, alg);

    status = psa_set_key_policy(key_handle, &policy);
    if (status != PSA_SUCCESS)
    {
       return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

   /* Import key */
   status = psa_import_key(key_handle, key_type, private_key, private_key_length);
   if (status != PSA_SUCCESS)
   {
       return(NX_CRYPTO_NOT_SUCCESSFUL);
   }

#elif (TFM_1_3 == 1)
    psa_set_key_usage_flags(&attributes, usage);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);

    /* Import the AES key into TF-M crypto partition. */
    status = psa_import_key(&attributes,
                            private_key, private_key_length,
                            &key_handle);
   if (status != PSA_SUCCESS)
   {
       return(NX_CRYPTO_NOT_SUCCESSFUL);
   }

#endif

   rsa_ptr -> nx_crypto_rsa_psa_key_handle = key_handle;
   return(NX_CRYPTO_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_rsa_psa_sign                             PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function computes a signature of the hash data using the       */
/*    private key.                                                        */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rsa_ptr                               RSA context                   */
/*    hash                                  Hash data to be signed        */
/*    hash_length                           Length of hash data           */
/*    private_key                           Pointer to EC private key     */
/*    signature                             Pointer to signature output   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_rsa_psa_sign(NX_CRYPTO_RSA_PSA *rsa_ptr, UCHAR *hash, UINT hash_length,
                                              UCHAR *private_key, UINT private_key_length,
                                              UCHAR *signature, UINT signature_length,
                                              UINT *actual_signature_length)
{
psa_status_t psa_status;
psa_algorithm_t alg;

    /* Select our PSA algorithm based on input hash. */
    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256);
    /*alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_384);
    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_512);*/

    rsa_ptr -> nx_crypto_rsa_psa_alg = alg;

    /* Check for key import if required. */
/*
    if (!rsa_ptr->nx_crypto_rsa_psa_key_handle)
    {
        nx_status = _nx_crypto_rsa_psa_import_key(rsa_ptr, alg, key_type, private_key, private_key_length);
        if (nx_status)
        {
            return(nx_status);
        }
    }
    else
*/
    {
        /* Assume the handle was provided as an opaque key value by the application. */
        rsa_ptr->nx_crypto_rsa_psa_key_handle = (psa_key_handle_t)(*private_key);
    }

    /* Encrypto the provided hash to sign it. */
    psa_status = psa_asymmetric_sign(rsa_ptr->nx_crypto_rsa_psa_key_handle, alg, (const uint8_t *)hash, hash_length,
                                     signature, signature_length, actual_signature_length);

    if (psa_status)
    {
      return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    return NX_CRYPTO_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_rsa_psa_verify                           PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function verifies the signature of the hash data using the     */
/*    provided public key.                                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    rsa_ptr                               RSA context                   */
/*    hash                                  Hash data to be verified      */
/*    hash_length                           Length of hash data           */
/*    public_key                            Pointer to RSA public key     */
/*    signature                             Signature to be verified      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_rsa_psa_verify(NX_CRYPTO_RSA_PSA *rsa_ptr, UCHAR *hash, UINT hash_length,
                                              UCHAR *public_key, UINT public_key_length,
                                              UCHAR *signature, UINT signature_length)
{
psa_status_t psa_status;

    if (!rsa_ptr->nx_crypto_rsa_psa_key_handle || rsa_ptr->nx_crypto_rsa_psa_alg == 0)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    /* Verify the signature using the algorithm assigned to the RSA context. */
    psa_status = psa_asymmetric_verify(rsa_ptr->nx_crypto_rsa_psa_key_handle,
                                       rsa_ptr->nx_crypto_rsa_psa_alg,
                                       hash,
                                       hash_length,
                                       (const uint8_t *)signature,
                                       signature_length);
    if (psa_status != PSA_SUCCESS)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    return NX_CRYPTO_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_rsa_psa_init            PORTABLE C                */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the common crypto method init callback for         */
/*    Express Logic supported RSA cryptograhic algorithm.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    method                                Pointer to crypto method      */
/*    key                                   Pointer to key                */
/*    key_size_in_bits                      Length of key size in bits    */
/*    handler                               Returned crypto handler       */
/*    crypto_metadata                       Metadata area                 */
/*    crypto_metadata_size                  Size of the metadata area     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT  _nx_crypto_method_rsa_psa_init(struct  NX_CRYPTO_METHOD_STRUCT *method,
                                                      UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                      VOID  **handle,
                                                      VOID  *crypto_metadata,
                                                      ULONG crypto_metadata_size)
{

    NX_CRYPTO_PARAMETER_NOT_USED(key);
    NX_CRYPTO_PARAMETER_NOT_USED(key_size_in_bits);
    NX_CRYPTO_PARAMETER_NOT_USED(handle);

    NX_CRYPTO_STATE_CHECK

    if ((method == NX_CRYPTO_NULL) || (crypto_metadata == NX_CRYPTO_NULL))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((((ULONG)crypto_metadata) & 0x3) != 0)
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_RSA_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_rsa_psa_cleanup          PORTABLE C               */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function cleans up the crypto metadata.                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    crypto_metadata                       Crypto metadata               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    NX_CRYPTO_MEMSET                      Set the memory                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT  _nx_crypto_method_rsa_psa_cleanup(VOID *crypto_metadata)
{

    NX_CRYPTO_STATE_CHECK

#ifdef NX_SECURE_KEY_CLEAR
    if (!crypto_metadata)
        return (NX_CRYPTO_SUCCESS);

    /* Destroy the key */
    NX_CRYPTO_RSA_PSA *rsa = (NX_CRYPTO_RSA_PSA *)crypto_metadata;
    psa_destroy_key(rsa->nx_crypto_rsa_psa_key_handle);

    /* Clean up the crypto metadata.  */
    NX_CRYPTO_MEMSET(crypto_metadata, 0, sizeof(NX_CRYPTO_RSA_PSA));
#else
    NX_CRYPTO_PARAMETER_NOT_USED(crypto_metadata);
#endif/* NX_SECURE_KEY_CLEAR  */

    return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_rsa_psa_operation       PORTABLE C                */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs an RSA operation.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    op                                    RSA operation                 */
/*    handle                                Crypto handle                 */
/*    method                                Cryption Method Object        */
/*    key                                   Encryption Key                */
/*    key_size_in_bits                      Key size in bits              */
/*    input                                 Input data                    */
/*    input_length_in_byte                  Input data size               */
/*    iv_ptr                                Initial vector                */
/*    output                                Output buffer                 */
/*    output_length_in_byte                 Output buffer size            */
/*    crypto_metadata                       Metadata area                 */
/*    crypto_metadata_size                  Metadata area size            */
/*    packet_ptr                            Pointer to packet             */
/*    nx_crypto_hw_process_callback         Callback function pointer     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _nx_crypto_rsa_psasign_psa_crypto     Sign using RSA                */
/*    _nx_crypto_rsa_psaverify_psa_crypto   Verify RSA signature          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_method_rsa_psa_operation(UINT op,
                                                          VOID *handle,
                                                          struct NX_CRYPTO_METHOD_STRUCT *method,
                                                          UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                          UCHAR *input, ULONG input_length_in_byte,
                                                          UCHAR *iv_ptr,
                                                          UCHAR *output, ULONG output_length_in_byte,
                                                          VOID *crypto_metadata, ULONG crypto_metadata_size,
                                                          VOID *packet_ptr,
                                                          VOID (*nx_crypto_hw_process_callback)(VOID *, UINT))
{
NX_CRYPTO_RSA_PSA *rsa;
UINT                 status = NX_CRYPTO_SUCCESS;
NX_CRYPTO_EXTENDED_OUTPUT
                    *extended_output;

    NX_CRYPTO_PARAMETER_NOT_USED(handle);
    NX_CRYPTO_PARAMETER_NOT_USED(iv_ptr);
    NX_CRYPTO_PARAMETER_NOT_USED(packet_ptr);
    NX_CRYPTO_PARAMETER_NOT_USED(nx_crypto_hw_process_callback);

    NX_CRYPTO_STATE_CHECK

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((method == NX_CRYPTO_NULL) || (crypto_metadata == NX_CRYPTO_NULL) || ((((ULONG)crypto_metadata) & 0x3) != 0))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_RSA_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    rsa = (NX_CRYPTO_RSA_PSA *)crypto_metadata;

    if (op == NX_CRYPTO_AUTHENTICATE)
    {
        if (key == NX_CRYPTO_NULL)
        {
            return(NX_CRYPTO_PTR_ERROR);
        }

        extended_output = (NX_CRYPTO_EXTENDED_OUTPUT *)output;

        /* Sign the input hash using the protected private key. */
        status = _nx_crypto_rsa_psa_sign(rsa,
                                         input,
                                         input_length_in_byte,
                                         key,
                                         key_size_in_bits >> 3,
                                         extended_output -> nx_crypto_extended_output_data,
                                         extended_output -> nx_crypto_extended_output_length_in_byte,
                                         (UINT *)&extended_output -> nx_crypto_extended_output_actual_size);
    }
    else if (op == NX_CRYPTO_VERIFY)
    {
        if (key == NX_CRYPTO_NULL)
        {
            return(NX_CRYPTO_PTR_ERROR);
        }

        status = _nx_crypto_rsa_psa_verify(rsa,
                                           input,
                                           input_length_in_byte,
                                           key,
                                           key_size_in_bits >> 3,
                                           output, output_length_in_byte);
    }
    else
    {
        status = NX_CRYPTO_NOT_SUCCESSFUL;
    }

    return(status);
}
