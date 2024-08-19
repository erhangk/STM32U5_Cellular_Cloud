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

#include "nx_crypto_sha_psa.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_sha_psa_initialize                    PORTABLE C         */
/*                                                           6.x          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function initializes the SHA context. It must be called        */
/*    prior to creating a SHA digest.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               SHA PSA context pointer       */
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
/*    _nx_crypto_method_sha_psa_operation    Handle hash operation        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT  _nx_crypto_sha_psa_initialize(NX_CRYPTO_SHA_PSA *context, UINT algorithm )
{
psa_status_t status;

    /* Determine if the context is non-null.  */
    if (context == NX_CRYPTO_NULL)
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    status = psa_hash_setup(&context -> nx_crypto_hash_psa_operation, PSA_ALG_SHA_256);

    /* Return success.  */
    return(status);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_sha_psa_update                           PORTABLE C      */
/*                                                           6.x          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function updates the digest with new input from the caller.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Hash context pointer          */
/*    input_ptr                             Pointer to input data         */
/*    input_length                          Number of bytes in input      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    psa_hash_update                       Process complete buffer       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _nx_crypto_method_sha_psa_operation    Handle hash operation        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_sha_psa_update(NX_CRYPTO_SHA_PSA *context, UCHAR *input_ptr, UINT input_length)
{
psa_status_t status;

    /* Determine if the context is non-null.  */
    if (context == NX_CRYPTO_NULL)
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    /* Determine if there is a length.  */
    if (input_length == 0)
    {
        return(NX_CRYPTO_SUCCESS);
    }

    status = psa_hash_update(&context -> nx_crypto_hash_psa_operation, input_ptr, input_length);

    /* Return success.  */
    return(status);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_sha_psa_digest_calculate                 PORTABLE C      */
/*                                                           6.x          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates the final digest. It is called when there  */
/*    is no more input to the digest and returns the digest to the caller.*/
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Hash context pointer          */
/*    digest                                Pointer to return buffer      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    psa_hash_finish                       Final update to the digest    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _nx_crypto_method_sha_psa_operation    Handle hash operation        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_sha_psa_digest_calculate(NX_CRYPTO_SHA_PSA *context, UCHAR *digest, UINT algorithm)
{
psa_status_t status;
size_t hash_length = 0;

    status = psa_hash_finish(&context -> nx_crypto_hash_psa_operation, digest, 32, &hash_length);

    /* Return successful completion.  */
    return(status);
}



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_sha_psa_init                      PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the common crypto method init callback.            */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_sha_psa_init(struct  NX_CRYPTO_METHOD_STRUCT *method,
                                                   UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                   VOID  **handle,
                                                   VOID  *crypto_metadata,
                                                   ULONG crypto_metadata_size)
{

    NX_CRYPTO_PARAMETER_NOT_USED(key);
    NX_CRYPTO_PARAMETER_NOT_USED(key_size_in_bits);
    NX_CRYPTO_PARAMETER_NOT_USED(handle);

    if ((method == NX_CRYPTO_NULL) || (crypto_metadata == NX_CRYPTO_NULL))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((((ULONG)crypto_metadata) & 0x3) != 0)
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_SHA_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    NX_CRYPTO_MEMSET(crypto_metadata, 0, sizeof(NX_CRYPTO_SHA_PSA));

    return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_sha_psa_cleanup                   PORTABLE C      */
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
/*    psa_hash_abort                        Close the hash operation      */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_sha_psa_cleanup(VOID *crypto_metadata)
{

    NX_CRYPTO_STATE_CHECK

    if (!crypto_metadata)
        return (NX_CRYPTO_SUCCESS);

    psa_hash_abort(&((NX_CRYPTO_SHA_PSA *)crypto_metadata) -> nx_crypto_hash_psa_operation);

    /* Clean up the crypto metadata.  */
    NX_CRYPTO_MEMSET(crypto_metadata, 0, sizeof(NX_CRYPTO_SHA_PSA));


    return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_sha_psa_operation                PORTABLE C       */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs message digest operations using the SHA      */
/*    algorithms.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    op                                    Hash operation                */
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
/*    _nx_crypto_sha_psa_initialize         Initialize the hash context   */
/*    _nx_crypto_sha_psa_update             Update the digest             */
/*    _nx_crypto_sha_psa_digest_calculate   Calculate the digest          */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_sha_psa_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
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
                                                        VOID (*nx_crypto_hw_process_callback)(VOID *packet_ptr, UINT status))
{
NX_CRYPTO_SHA_PSA *ctx;

    NX_CRYPTO_PARAMETER_NOT_USED(handle);
    NX_CRYPTO_PARAMETER_NOT_USED(key);
    NX_CRYPTO_PARAMETER_NOT_USED(key_size_in_bits);
    NX_CRYPTO_PARAMETER_NOT_USED(iv_ptr);
    NX_CRYPTO_PARAMETER_NOT_USED(packet_ptr);
    NX_CRYPTO_PARAMETER_NOT_USED(nx_crypto_hw_process_callback);

    NX_CRYPTO_STATE_CHECK

    if (method == NX_CRYPTO_NULL)
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((crypto_metadata == NX_CRYPTO_NULL) || ((((ULONG)crypto_metadata) & 0x3) != 0))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_SHA_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    ctx = (NX_CRYPTO_SHA_PSA *)crypto_metadata;

    if ((method -> nx_crypto_algorithm != NX_CRYPTO_AUTHENTICATION_HMAC_SHA2_256) &&
        (method -> nx_crypto_algorithm != NX_CRYPTO_HASH_SHA224) &&
        (method -> nx_crypto_algorithm != NX_CRYPTO_HASH_SHA256))
    {
        /* Incorrect method. */
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    switch (op)
    {
    case NX_CRYPTO_HASH_INITIALIZE:
        _nx_crypto_sha_psa_initialize((NX_CRYPTO_SHA_PSA *)crypto_metadata, method -> nx_crypto_algorithm);
        break;

    case NX_CRYPTO_HASH_UPDATE:
        _nx_crypto_sha_psa_update((NX_CRYPTO_SHA_PSA *)crypto_metadata, input, input_length_in_byte);
        break;

    case NX_CRYPTO_HASH_CALCULATE:
        if ((method -> nx_crypto_algorithm == NX_CRYPTO_AUTHENTICATION_HMAC_SHA2_256) ||
            (method -> nx_crypto_algorithm == NX_CRYPTO_HASH_SHA256))
        {
            if (output_length_in_byte < 32)
                return(NX_CRYPTO_INVALID_BUFFER_SIZE);
        }
        else if (output_length_in_byte < 28)
            return(NX_CRYPTO_INVALID_BUFFER_SIZE);
        _nx_crypto_sha_psa_digest_calculate((NX_CRYPTO_SHA_PSA *)crypto_metadata, output,
                                           method -> nx_crypto_algorithm);
        break;

    default:
        if ((method -> nx_crypto_algorithm == NX_CRYPTO_AUTHENTICATION_HMAC_SHA2_256) ||
            (method -> nx_crypto_algorithm == NX_CRYPTO_HASH_SHA256))
        {
            if (output_length_in_byte < 32)
                return(NX_CRYPTO_INVALID_BUFFER_SIZE);
        }
        else if (output_length_in_byte < 28)
            return(NX_CRYPTO_INVALID_BUFFER_SIZE);

        _nx_crypto_sha_psa_initialize(ctx, method -> nx_crypto_algorithm);
        _nx_crypto_sha_psa_update(ctx, input, input_length_in_byte);
        _nx_crypto_sha_psa_digest_calculate(ctx, output, method -> nx_crypto_algorithm);
        break;
    }

    return NX_CRYPTO_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    nx_crypto_hash_clone_psa                           PORTABLE C       */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function clones a hash operation.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    dest_metadata                         Destination hash context      */
/*    source_metadata                       Source hash context           */
/*    length                                Metadata area size            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    psa_hash_clone                        Clone the hash context        */
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
UINT nx_crypto_hash_clone_psa(VOID *dest_metadata, VOID *source_metadata, ULONG length)
{
NX_CRYPTO_SHA_PSA *dst;
NX_CRYPTO_SHA_PSA *src;

    if (length != sizeof(NX_CRYPTO_SHA_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    dst = (NX_CRYPTO_SHA_PSA *)dest_metadata;
    NX_CRYPTO_MEMSET(dest_metadata, 0, sizeof(NX_CRYPTO_SHA_PSA));
    src = (NX_CRYPTO_SHA_PSA *)source_metadata;

    return(psa_hash_clone(&src -> nx_crypto_hash_psa_operation, &dst -> nx_crypto_hash_psa_operation));
}
