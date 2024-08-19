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
/**   AES Encryption - PSA/TF-M Driver                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#include "nx_crypto_aes_psa.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_aes_init                          PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function initializes the AES crypto module.                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    method                                Crypto Method Object          */
/*    key                                   Key                           */
/*    key_size_in_bits                      Size of the key, in bits      */
/*    handle                                Handle, specified by user     */
/*    crypto_metadata                       Metadata area                 */
/*    crypto_metadata_size                  Size of the metadata area     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    psa_import_key                        Set the key for AES           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _nx_crypto_method_aes_psa_operation   Handle AES encrypt or decrypt */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT  _nx_crypto_method_aes_psa_init(struct NX_CRYPTO_METHOD_STRUCT *method,
                                                    UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                    VOID **handle,
                                                    VOID *crypto_metadata,
                                                    ULONG crypto_metadata_size)
{
NX_CRYPTO_AES_PSA *ctx;
psa_status_t psa_status;
psa_key_usage_t usage;
#if (TFM_1_0 == 1)
psa_key_policy_t policy;
#elif (TFM_1_3 == 1)
psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
#else
#error "Must define a TF-M version"
#endif

    NX_CRYPTO_PARAMETER_NOT_USED(handle);

    NX_CRYPTO_STATE_CHECK


    if ((method == NX_CRYPTO_NULL) || (key == NX_CRYPTO_NULL) || (crypto_metadata == NX_CRYPTO_NULL))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((((ULONG)crypto_metadata) & 0x3) != 0)
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_AES_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    /* We only support 128-bit or 256-bit key size for the time-being. */
    if ((key_size_in_bits != NX_CRYPTO_AES_128_KEY_LEN_IN_BITS) && (key_size_in_bits != NX_CRYPTO_AES_192_KEY_LEN_IN_BITS) && (key_size_in_bits != NX_CRYPTO_AES_256_KEY_LEN_IN_BITS))
    {
        return(NX_CRYPTO_UNSUPPORTED_KEY_SIZE);
    }

    /* Clear out the control structure. */
    NX_CRYPTO_MEMSET(((NX_CRYPTO_AES_PSA *)crypto_metadata), 0, sizeof(NX_CRYPTO_AES_PSA));

    /* Setup the structure for use. */
    ctx = (NX_CRYPTO_AES_PSA *)crypto_metadata;
    ctx->nx_crypto_aes_psa_algorithm = 0;
    ctx->nx_crypto_aes_psa_operation = psa_cipher_operation_init();
    ctx->nx_crypto_aes_key_size = key_size_in_bits >> 3;
    ctx->nx_crypto_aes_psa_initialized = 0;

#if (TFM_1_0 == 1)
    /* Allocate a transient key */
    psa_status = psa_allocate_key(&ctx -> nx_crypto_aes_key_handle);
    if (psa_status != PSA_SUCCESS)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }
#endif

    /* Determine our algorithm type for setting the key policy. */
    if (method -> nx_crypto_algorithm == NX_CRYPTO_ENCRYPTION_AES_CBC)
    {
        /* Set the algorithm - we are using AES-CBC with PKCS7 padding. */
        ctx -> nx_crypto_aes_psa_algorithm = PSA_ALG_CBC_NO_PADDING; //PSA_ALG_CBC_PKCS7;
    }
    else
    {
        psa_destroy_key(ctx -> nx_crypto_aes_key_handle);
        return(NX_CRYPTO_INVALID_ALGORITHM);
    }

    /* Setup the key policy */
    usage = (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);


#if (TFM_1_0 == 1)
    policy = psa_key_policy_init();
    psa_key_policy_set_usage(&policy, usage, ctx -> nx_crypto_aes_psa_algorithm);
    psa_status = psa_set_key_policy(ctx -> nx_crypto_aes_key_handle, &policy);
    if (psa_status != PSA_SUCCESS)
    {
        psa_destroy_key(ctx -> nx_crypto_aes_key_handle);
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    /* Import the AES key into TF-M crypto partition. */
    psa_status = psa_import_key(ctx -> nx_crypto_aes_key_handle,
                                PSA_KEY_TYPE_AES,
                                key, ctx -> nx_crypto_aes_key_size);

#elif (TFM_1_3 == 1)
    psa_set_key_usage_flags(&attributes, usage);
    psa_set_key_algorithm(&attributes, ctx -> nx_crypto_aes_psa_algorithm);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);

    /* Import the AES key into TF-M crypto partition. */
    psa_status = psa_import_key(&attributes,
                                key, ctx -> nx_crypto_aes_key_size,
                                &ctx -> nx_crypto_aes_key_handle);

#endif


    if (psa_status != PSA_SUCCESS)
    {
        psa_destroy_key(ctx -> nx_crypto_aes_key_handle);
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }
    return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_aes_psa_cleanup                   PORTABLE C      */
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
/*    NX_CRYPTO_MEMSET                      Clear memory                  */
/*    psa_cipher_abort                      Cleanup and abort PSA cipher  */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_aes_psa_cleanup(VOID *crypto_metadata)
{
NX_CRYPTO_AES_PSA *ctx;
psa_status_t psa_status;
UINT    status = NX_CRYPTO_SUCCESS;

    NX_CRYPTO_STATE_CHECK

    if ((crypto_metadata == NX_CRYPTO_NULL) || ((((ULONG)crypto_metadata) & 0x3) != 0))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    ctx = (NX_CRYPTO_AES_PSA *)crypto_metadata;

    /* Abort the operation - PSA allows this to be called on any operation
       regardless of state so it's safe to abort even if the operation is
       already finished. */
    psa_status = psa_cipher_abort(&ctx->nx_crypto_aes_psa_operation);

    if (psa_status != PSA_SUCCESS)
    {
        status = NX_CRYPTO_NOT_SUCCESSFUL;
    }

    /* Destroy/deallocate our PSA key. */
    psa_status = psa_destroy_key(ctx -> nx_crypto_aes_key_handle);
    if (psa_status != PSA_SUCCESS)
    {
        status = NX_CRYPTO_NOT_SUCCESSFUL;
    }

    ctx->nx_crypto_aes_psa_initialized = 0;

    return(status);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_aes_psa_setup                     PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Helper function to setup a PSA cipher context for AES.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ctx                                   NetX PSA AES control block    */
/*    op                                    AES operation                 */
/*    iv_ptr                                AES initialization vector     */
/*    iv_size                               Size of IV                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    psa_cipher_decrypt_setup              PSA/TF-M cipher setup         */
/*    psa_cipher_encrypt_setup              PSA/TF-M cipher setup         */
/*    psa_cipher_set_iv                     PSA/TF-M set cipher IV        */
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
static NX_CRYPTO_KEEP UINT  _nx_crypto_method_aes_psa_setup(NX_CRYPTO_AES_PSA *ctx, UINT op, UCHAR *iv, UINT iv_size)
{
psa_status_t psa_status = PSA_SUCCESS;

        /* If we are calling setup on a previously initialized PSA operation, we want to clear out the
           operation with a call to psa_cipher_abort and then re-initialize with the same key but a new IV. */
        if (ctx->nx_crypto_aes_psa_initialized)
        {
            ctx->nx_crypto_aes_psa_initialized = 0;
            psa_status = psa_cipher_abort(&ctx->nx_crypto_aes_psa_operation);
        }

        if (psa_status != PSA_SUCCESS)
        {
            return(NX_CRYPTO_NOT_SUCCESSFUL);
        }

        /* Setup PSA/TF-M for AES encryption or decryption. */
        if (NX_CRYPTO_DECRYPT == op)
        {
           psa_status = psa_cipher_decrypt_setup(&ctx->nx_crypto_aes_psa_operation, ctx->nx_crypto_aes_key_handle,
                                                 ctx->nx_crypto_aes_psa_algorithm);
        }
        else if (NX_CRYPTO_ENCRYPT == op)
        {
            psa_status = psa_cipher_encrypt_setup(&ctx->nx_crypto_aes_psa_operation, ctx->nx_crypto_aes_key_handle,
                                                  ctx->nx_crypto_aes_psa_algorithm);
        }

        if (psa_status != PSA_SUCCESS)
        {
            return(NX_CRYPTO_NOT_SUCCESSFUL);
        }

        /* Initialize AES with the IV generated by TLS. */
        psa_status = psa_cipher_set_iv(&ctx->nx_crypto_aes_psa_operation, iv, iv_size);
        if (psa_status != PSA_SUCCESS)
        {
            return(NX_CRYPTO_NOT_SUCCESSFUL);
        }

        /* The PSA operation is initialized, mark it so we don't call psa_cipher_*_setup on an in-use operation. */
        ctx->nx_crypto_aes_psa_initialized = 1;

        return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_aes_psa_operation                 PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function encrypts and decrypts a message using                 */
/*    the AES algorithm through the PSA/TF-M cryptography interface.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    op                                    AES operation                 */
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
/*    _nx_crypto_method_aes_psa_cbc_operation                             */
/*                                          Handle AES CBC operation      */
/*    _nx_crypto_method_aes_psa_gcm_operation                             */
/*                                          Handle AES GCM operation      */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_aes_psa_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
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
UINT    status;

    NX_CRYPTO_STATE_CHECK

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((method == NX_CRYPTO_NULL) || (crypto_metadata == NX_CRYPTO_NULL) || ((((ULONG)crypto_metadata) & 0x3) != 0))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_AES_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    /* Check if the algorithm is cbc or ctr. */
    if (method -> nx_crypto_algorithm == NX_CRYPTO_ENCRYPTION_AES_CBC)
    {

        /* AES CBC */
        status = _nx_crypto_method_aes_psa_cbc_operation(op, handle, method, key, key_size_in_bits,
                                                     input, input_length_in_byte, iv_ptr,
                                                     output, output_length_in_byte,
                                                     crypto_metadata, crypto_metadata_size,
                                                     packet_ptr, nx_crypto_hw_process_callback);
    }
#if 0 /* TODO: AES-GCM for PSA/TF-M */
    else if ((method -> nx_crypto_algorithm >= NX_CRYPTO_ENCRYPTION_AES_GCM_8) &&
             (method -> nx_crypto_algorithm <= NX_CRYPTO_ENCRYPTION_AES_GCM_16))
    {

        /* AES GCM */
        status = _nx_crypto_method_aes_gcm_operation(op, handle, method, key, key_size_in_bits,
                                                     input, input_length_in_byte, iv_ptr,
                                                     output, output_length_in_byte,
                                                     crypto_metadata, crypto_metadata_size,
                                                     packet_ptr, nx_crypto_hw_process_callback);

    }
#endif
    else
    {
        status = NX_CRYPTO_INVALID_ALGORITHM;
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_aes_psa_cbc_operation             PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function encrypts and decrypts a message using                 */
/*    the AES CBC algorithm.                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    op                                    AES operation                 */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_aes_psa_cbc_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
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

NX_CRYPTO_AES_PSA *ctx;
psa_status_t psa_status;
size_t output_bytes = 0;
UINT    status = NX_CRYPTO_NOT_SUCCESSFUL;

    NX_CRYPTO_PARAMETER_NOT_USED(handle);
    NX_CRYPTO_PARAMETER_NOT_USED(key);
    NX_CRYPTO_PARAMETER_NOT_USED(key_size_in_bits);
    NX_CRYPTO_PARAMETER_NOT_USED(output_length_in_byte);
    NX_CRYPTO_PARAMETER_NOT_USED(packet_ptr);
    NX_CRYPTO_PARAMETER_NOT_USED(nx_crypto_hw_process_callback);

    NX_CRYPTO_STATE_CHECK

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((method == NX_CRYPTO_NULL) || (crypto_metadata == NX_CRYPTO_NULL) || ((((ULONG)crypto_metadata) & 0x3) != 0))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_AES_PSA))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    ctx = (NX_CRYPTO_AES_PSA *)crypto_metadata;

    switch (op)
    {
        case NX_CRYPTO_ENCRYPT:
        {
            status = _nx_crypto_method_aes_psa_setup(ctx, op, iv_ptr, method -> nx_crypto_IV_size_in_bits >> 3);
            if (status != NX_CRYPTO_SUCCESS)
            {
                break;
            }

            /* Encrypt the data using the TF-M crypto service. */
            psa_status = psa_cipher_update(&ctx->nx_crypto_aes_psa_operation,
                                           input, input_length_in_byte,
                                           output, output_length_in_byte,
                                           &output_bytes);

            if (psa_status != PSA_SUCCESS)
            {
                /* Abort the operation per TF-M spec. */
                psa_cipher_abort(&ctx->nx_crypto_aes_psa_operation);

                status = NX_CRYPTO_NOT_SUCCESSFUL;
                break;
            }
        }
        break;

        case NX_CRYPTO_DECRYPT:
        {
            status = _nx_crypto_method_aes_psa_setup(ctx, op, iv_ptr, method -> nx_crypto_IV_size_in_bits >> 3);
            if (status != NX_CRYPTO_SUCCESS)
            {
                break;
            }

            /* Encrypt the data using the TF-M crypto service. */
            psa_status = psa_cipher_update(&ctx->nx_crypto_aes_psa_operation,
                                           input, input_length_in_byte,
                                           output, output_length_in_byte,
                                           &output_bytes);

            if (psa_status != PSA_SUCCESS)
            {
                /* Abort the operation per TF-M spec. */
                psa_cipher_abort(&ctx->nx_crypto_aes_psa_operation);

                status = NX_CRYPTO_NOT_SUCCESSFUL;
                break;
            }

            psa_status = psa_cipher_finish(&ctx->nx_crypto_aes_psa_operation,
                                           output, output_length_in_byte,
                                           &output_bytes);

            if (psa_status != PSA_SUCCESS)
            {
                /* Abort the operation per TF-M spec. */
                psa_cipher_abort(&ctx->nx_crypto_aes_psa_operation);

                status = NX_CRYPTO_NOT_SUCCESSFUL;
                break;
            }

            status = NX_CRYPTO_SUCCESS;
        }
        break;

        case NX_CRYPTO_DECRYPT_INITIALIZE:
        {
            status = _nx_crypto_method_aes_psa_setup(ctx, NX_CRYPTO_DECRYPT, iv_ptr, method -> nx_crypto_IV_size_in_bits >> 3);
        }
        break;

        case NX_CRYPTO_ENCRYPT_INITIALIZE:
        {
            status = _nx_crypto_method_aes_psa_setup(ctx, NX_CRYPTO_ENCRYPT, iv_ptr, method -> nx_crypto_IV_size_in_bits >> 3);
        }
        break;

        case NX_CRYPTO_DECRYPT_UPDATE:
        /* fallthrough - PSA encrypt and decrypt use the same update routine*/
        case NX_CRYPTO_ENCRYPT_UPDATE:
        {
            /* Encrypt the data using the TF-M crypto service. */
            psa_status = psa_cipher_update(&ctx->nx_crypto_aes_psa_operation,
                                           input, input_length_in_byte,
                                           output, output_length_in_byte,
                                           &output_bytes);

            if (psa_status != PSA_SUCCESS)
            {
                /* Abort the operation per TF-M spec. */
                psa_cipher_abort(&ctx->nx_crypto_aes_psa_operation);

                status = NX_CRYPTO_NOT_SUCCESSFUL;
                break;
            }
            status = NX_CRYPTO_SUCCESS;
        } break;

        case NX_CRYPTO_ENCRYPT_CALCULATE:
            status = NX_CRYPTO_SUCCESS;
            break;
        /* fallthrough */
        case NX_CRYPTO_DECRYPT_CALCULATE:
            status = NX_CRYPTO_SUCCESS;
            break;
#if 0
            {
            psa_status = psa_cipher_finish(&ctx->nx_crypto_aes_psa_operation,
                                           output, output_length_in_byte,
                                           &output_bytes);

            if (psa_status != PSA_SUCCESS)
            {
                /* Abort the operation per TF-M spec. */
                psa_cipher_abort(&ctx->nx_crypto_aes_psa_operation);

                status = NX_CRYPTO_NOT_SUCCESSFUL;
                break;
            }

            status = NX_CRYPTO_SUCCESS;
        } break;
#endif
        default:
        {
            status = NX_CRYPTO_INVALID_ALGORITHM;
        } break;
    }

    return(status);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_aes_psa_gcm_operation             PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function encrypts and decrypts a message using                 */
/*    the AES GCM algorithm.                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    op                                    AES operation                 */
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
/*    _nx_crypto_gcm_operation              Perform GCM encryption or     */
/*                                            decryption                  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _nx_crypto_method_aes_operation       Handle AES encrypt or decrypt */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
#if 0 /* TODO: AES-GCM for TF-M/PSA */
NX_CRYPTO_KEEP UINT  _nx_crypto_method_aes_gcm_operation(UINT op,      /* Encrypt, Decrypt, Authenticate */
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

NX_CRYPTO_AES *ctx;
UINT icv_len;
UINT message_len;
UINT    status;

    NX_CRYPTO_PARAMETER_NOT_USED(handle);
    NX_CRYPTO_PARAMETER_NOT_USED(key);
    NX_CRYPTO_PARAMETER_NOT_USED(key_size_in_bits);
    NX_CRYPTO_PARAMETER_NOT_USED(output_length_in_byte);
    NX_CRYPTO_PARAMETER_NOT_USED(packet_ptr);
    NX_CRYPTO_PARAMETER_NOT_USED(nx_crypto_hw_process_callback);

    NX_CRYPTO_STATE_CHECK

    /* Verify the metadata addrsss is 4-byte aligned. */
    if ((method == NX_CRYPTO_NULL) || (crypto_metadata == NX_CRYPTO_NULL) || ((((ULONG)crypto_metadata) & 0x3) != 0))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_AES))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    ctx = (NX_CRYPTO_AES *)crypto_metadata;

    if ((method -> nx_crypto_algorithm < NX_CRYPTO_ENCRYPTION_AES_GCM_8) ||
        (method -> nx_crypto_algorithm > NX_CRYPTO_ENCRYPTION_AES_GCM_16))
    {
        return(NX_CRYPTO_INVALID_ALGORITHM);
    }

    /* IV : Nonce length(1 byte) + Nonce
       nx_crypto_ICV_size_in_bits: authentication tag length in bits */
    switch (op)
    {
        case NX_CRYPTO_DECRYPT:
        {
            if (iv_ptr == NX_CRYPTO_NULL ||
                (ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data_len > 0 &&
                 ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data == NX_CRYPTO_NULL))
            {
                status = NX_CRYPTO_PTR_ERROR;
                break;
            }

            icv_len = (method -> nx_crypto_ICV_size_in_bits >> 3);

            if (input_length_in_byte < icv_len || output_length_in_byte < input_length_in_byte - icv_len)
            {
                status = NX_CRYPTO_INVALID_BUFFER_SIZE;
                break;
            }

            message_len = input_length_in_byte - icv_len;
            status = _nx_crypto_gcm_decrypt_init(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                 (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psaencrypt,
                                                 ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data,
                                                 ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data_len,
                                                 iv_ptr, NX_CRYPTO_AES_BLOCK_SIZE);

            if (status)
            {
                break;
            }

            status = _nx_crypto_gcm_decrypt_update(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                   (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psaencrypt,
                                                   input, output, message_len,
                                                   NX_CRYPTO_AES_BLOCK_SIZE);

            if (status)
            {
                break;
            }

            status = _nx_crypto_gcm_decrypt_calculate(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                      (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psaencrypt,
                                                      input + message_len, icv_len,
                                                      NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_ENCRYPT:
        {
            if (iv_ptr == NX_CRYPTO_NULL ||
                (ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data_len > 0 &&
                 ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data == NX_CRYPTO_NULL))
            {
                status = NX_CRYPTO_PTR_ERROR;
                break;
            }

            icv_len = (method -> nx_crypto_ICV_size_in_bits >> 3);

            if (output_length_in_byte < input_length_in_byte + icv_len)
            {
                status = NX_CRYPTO_INVALID_BUFFER_SIZE;
                break;
            }

            status = _nx_crypto_gcm_encrypt_init(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                 (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psaencrypt,
                                                 ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data,
                                                 ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data_len,
                                                 iv_ptr, NX_CRYPTO_AES_BLOCK_SIZE);

            if (status)
            {
                break;
            }

            status = _nx_crypto_gcm_encrypt_update(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                   (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psaencrypt,
                                                   input, output, input_length_in_byte,
                                                   NX_CRYPTO_AES_BLOCK_SIZE);

            if (status)
            {
                break;
            }

            status = _nx_crypto_gcm_encrypt_calculate(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                      (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psaencrypt,
                                                      output + input_length_in_byte, icv_len,
                                                      NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_DECRYPT_INITIALIZE:
        {
            if (iv_ptr == NX_CRYPTO_NULL)
            {
                status = NX_CRYPTO_PTR_ERROR;
                break;
            }

            status = _nx_crypto_gcm_decrypt_init(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                 (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psa_encrypt,
                                                 input, /* pointers to AAD */
                                                 input_length_in_byte, /* length of AAD */
                                                 iv_ptr, NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_DECRYPT_UPDATE:
        {
            status = _nx_crypto_gcm_decrypt_update(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                   (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psa_encrypt,
                                                   input, output, input_length_in_byte,
                                                   NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_DECRYPT_CALCULATE:
        {
            icv_len = (method -> nx_crypto_ICV_size_in_bits >> 3);
            if (input_length_in_byte < icv_len)
            {
                status = NX_CRYPTO_INVALID_BUFFER_SIZE;
                break;
            }

            status = _nx_crypto_gcm_decrypt_calculate(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                      (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psa_encrypt,
                                                      input, icv_len,
                                                      NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_ENCRYPT_INITIALIZE:
        {
            if (iv_ptr == NX_CRYPTO_NULL)
            {
                status = NX_CRYPTO_PTR_ERROR;
                break;
            }

            status = _nx_crypto_gcm_encrypt_init(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                 (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psa_encrypt,
                                                 input, /* pointers to AAD */
                                                 input_length_in_byte, /* length of AAD */
                                                 iv_ptr, NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_ENCRYPT_UPDATE:
        {
            status = _nx_crypto_gcm_encrypt_update(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                   (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psa_encrypt,
                                                   input, output, input_length_in_byte,
                                                   NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_ENCRYPT_CALCULATE:
        {
            icv_len = (method -> nx_crypto_ICV_size_in_bits >> 3);
            if (output_length_in_byte < icv_len)
            {
                status = NX_CRYPTO_INVALID_BUFFER_SIZE;
                break;
            }

            status = _nx_crypto_gcm_encrypt_calculate(ctx, &(ctx -> nx_crypto_aes_mode_context.gcm),
                                                      (UINT (*)(VOID *, UCHAR *, UCHAR *, UINT))_nx_crypto_aes_psa_encrypt,
                                                      output, icv_len,
                                                      NX_CRYPTO_AES_BLOCK_SIZE);
        } break;

        case NX_CRYPTO_SET_ADDITIONAL_DATA:
        {

            /* Set additional data pointer.  */
            ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data = (VOID *)input;

            /* Set additional data length.  */
            ctx -> nx_crypto_aes_mode_context.gcm.nx_crypto_gcm_additional_data_len = input_length_in_byte;

            status = NX_CRYPTO_SUCCESS;
        } break;

        default:
        {
            status = NX_CRYPTO_INVALID_ALGORITHM;
        } break;
    }

    return(status);
}
#endif