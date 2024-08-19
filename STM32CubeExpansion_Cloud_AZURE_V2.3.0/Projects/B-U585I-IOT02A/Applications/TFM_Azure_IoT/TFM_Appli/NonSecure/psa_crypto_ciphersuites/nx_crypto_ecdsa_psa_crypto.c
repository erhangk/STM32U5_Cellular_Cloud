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

#include "nx_crypto_ecdsa_psa_crypto.h"

/* TODO: determine a better way to select a Key ID here. */
#define _TEMP_KEY_ID (1234U)

static UINT _nx_crypto_ecdsa_psa_import_key(NX_CRYPTO_ECDSA_PSA_CRYPTO *ecdsa_ptr, psa_algorithm_t alg, psa_key_type_t key_type,
                                            uint8_t *import_key, size_t import_key_length)
{
psa_key_handle_t key_handle;
psa_status_t status;

psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
psa_key_usage_t usage = (PSA_KEY_USAGE_SIGN | PSA_KEY_USAGE_VERIFY);
size_t key_len;

    key_len = import_key_length;

    psa_set_key_usage_flags(&attributes, usage);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_id(&attributes, _TEMP_KEY_ID);

    /* Import the AES key into TF-M crypto partition. */
    status = psa_import_key(&attributes,
                            import_key, key_len,
                            &key_handle);

   if (status != PSA_SUCCESS) {
       return(NX_CRYPTO_NOT_SUCCESSFUL);
   }

   ecdsa_ptr->key_id = _TEMP_KEY_ID;
   return(NX_CRYPTO_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_ecdsa_sign_psa_crypto                   PORTABLE C      */
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
/*    ecdsa_ptr                             ECDSA context                 */
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
NX_CRYPTO_KEEP UINT _nx_crypto_ecdsa_sign_psa_crypto(NX_CRYPTO_ECDSA_PSA_CRYPTO *ecdsa_ptr, UCHAR *hash, UINT hash_length,
                                                     UCHAR *private_key, UINT private_key_length,
                                                     UCHAR *signature, UINT signature_length,
                                                     UINT *actual_signature_length)
{
//UINT nx_status = NX_CRYPTO_SUCCESS;
psa_status_t psa_status;
NX_CRYPTO_EC *curve = ecdsa_ptr->nx_crypto_ecdsa_curve;
psa_algorithm_t alg;
//psa_key_type_t key_type;
UINT imported_key = 0;
UINT i;
UINT curve_size;
UINT r_size;
UINT s_size;
UCHAR pad_zero_r;
UCHAR pad_zero_s;
UINT sequence_size;
UCHAR *signature_r;
UCHAR *signature_s;
psa_key_handle_t key_handle = {0};

    /* Set algorithm and key type for private key based on curve id */
    switch (curve->nx_crypto_ec_id)
    {
        case NX_CRYPTO_EC_SECP256R1:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
          //  key_type = PSA_KEY_TYPE_ECC_KEYPAIR(PSA_ECC_CURVE_SECP256R1);
            curve_size = 32;
            break;
        case NX_CRYPTO_EC_SECP384R1:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_384);
          //  key_type = PSA_KEY_TYPE_ECC_KEYPAIR(PSA_ECC_CURVE_SECP384R1);
            curve_size = 48;
            break;
        default:
            return(NX_CRYTPO_MISSING_ECC_CURVE);
    }
/*
    if (!ecdsa_ptr->key_handle)
    {
        nx_status = _nx_crypto_ecdsa_psa_import_key(ecdsa_ptr, alg, key_type, private_key, private_key_length);
        if (nx_status)
        {
            return(nx_status);
        }
        imported_key = 1;
    }
    else
*/
    {
        /* TF-M based key - it's already been imported. */
        if (private_key_length != sizeof(uint32_t))
        {
            return(NX_CRYPTO_NOT_SUCCESSFUL);
        }
        ecdsa_ptr -> key_id = *((uint32_t*)(private_key));
    }

    /* Open our key based on the passed-in Key ID. */
    psa_status = psa_open_key(ecdsa_ptr -> key_id, &key_handle);

    if (psa_status)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    /* Perform the signing operation. */
    psa_status = psa_asymmetric_sign(key_handle, alg, (const uint8_t *)hash, hash_length,
                                     ecdsa_ptr -> nx_crypto_ecdsa_scratch,
                                     sizeof(ecdsa_ptr -> nx_crypto_ecdsa_scratch), actual_signature_length);

    if (psa_status)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    /* Don't destroy the key if it wasn't imported during the init (hardware key type). */
    if (imported_key)
    {
        psa_status = psa_destroy_key(key_handle);
    }
    else
    {
        psa_status = psa_close_key(key_handle);
    }

    if (psa_status != PSA_SUCCESS)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    ecdsa_ptr->alg = alg;
/*
    ecdsa_ptr->nx_crypto_ecdsa_signature_length = (*actual_signature_length);
    UINT len = ecdsa_ptr->nx_crypto_ecdsa_signature_length;
    NX_CRYPTO_MEMCPY(ecdsa_ptr->nx_crypto_ecdsa_signature, signature, len);
*/

    /* Output r and s as two INTEGER in ASN.1 encoding */
    signature_r = ecdsa_ptr -> nx_crypto_ecdsa_scratch;
    r_size = curve_size;
    signature_s = signature_r + r_size;
    s_size = curve_size;

    /* Trim prefix zeros. */
    for (i = 0; i < r_size; i++)
    {
        if (signature_r[i])
        {

            /* Loop until none zero byte. */
            break;
        }
    }
    signature_r += i;
    r_size -= i;

    /* The most significant bit must be zero to indicate positive integer. */
    /* Pad zero at the front if necessary. */
    pad_zero_r = (signature_r[0] & 0x80) ? 1 : 0;

    for (i = 0; i < s_size; i++)
    {
        if (signature_s[i])
        {

            /* Loop until none zero byte. */
            break;
        }
    }
    signature_s += i;
    s_size -= i;

    /* The most significant bit must be zero to indicate positive integer. */
    /* Pad zero at the front if necessary. */
    pad_zero_s = (signature_s[0] & 0x80) ? 1 : 0;

    /* Size of sequence. */
    sequence_size = r_size + pad_zero_r + s_size + pad_zero_s + 4;

    signature[0] = 0x30;    /* SEQUENCE */
    if (sequence_size < 0x80)
    {
        signature[1] = (UCHAR)sequence_size;
        signature += 2;
        *actual_signature_length = sequence_size + 2;
    }
    else
    {
        signature[1] = 0x81;
        signature[2] = (UCHAR)sequence_size;
        signature += 3;
        *actual_signature_length = sequence_size + 3;
    }

    /* Setup r. */
    NX_CRYPTO_MEMCPY(&signature[2 + pad_zero_r], signature_r, r_size); /* Use case of memmove is verified. */
    signature[0] = 0x02;    /* Integer */
    signature[1] = (UCHAR)(r_size + pad_zero_r);
    if (pad_zero_r)
    {
        signature[2] = 0;
    }
    signature += (2u + pad_zero_r + r_size);

    /* Setup s. */
    NX_CRYPTO_MEMCPY(&signature[2 + pad_zero_s], signature_s, s_size); /* Use case of memmove is verified. */
    signature[0] = 0x02;    /* Integer */
    signature[1] = (UCHAR)(s_size + pad_zero_s);
    if (pad_zero_s)
    {
        signature[2] = 0;
    }

    return NX_CRYPTO_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_ecdsa_verify_psa_crypto                 PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function verifies the signature of the hash data using the     */
/*    public key.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ecdsa_ptr                             ECDSA context                 */
/*    hash                                  Hash data to be verified      */
/*    hash_length                           Length of hash data           */
/*    public_key                            Pointer to EC public key      */
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
NX_CRYPTO_KEEP UINT _nx_crypto_ecdsa_verify_psa_crypto(NX_CRYPTO_ECDSA_PSA_CRYPTO *ecdsa_ptr, UCHAR *hash, UINT hash_length,
                                                       UCHAR *public_key, UINT public_key_length,
                                                       UCHAR *signature, UINT signature_length)
{
UINT nx_status = NX_CRYPTO_SUCCESS;
psa_status_t psa_status;
NX_CRYPTO_EC *curve = ecdsa_ptr->nx_crypto_ecdsa_curve;
psa_algorithm_t alg;
psa_key_type_t key_type;
UINT imported_key = 0;
UINT curve_size;
psa_key_handle_t key_handle = {0};

    /* Check the size in SEQUENCE.  */
    if (signature[1] & 0x80)
    {
        if (signature[1] != 0x81 || signature_length < (signature[2] + 3u))
        {
            return(NX_CRYPTO_SIZE_ERROR);
        }
        signature_length = signature[2];
        signature += 3;
    }
    else
    {
        if (signature_length < (signature[1] + 2u))
        {
            return(NX_CRYPTO_SIZE_ERROR);
        }
        signature_length = signature[1];
        signature += 2;
    }


    /* Set algorithm and key type for public key based on curve id - this is a pure public key
       and NOT an ECC key pair (see PSA sign operation) */
    switch (curve->nx_crypto_ec_id)
    {
        case NX_CRYPTO_EC_SECP256R1:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
            key_type = PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_CURVE_SECP256R1);
            curve_size = 32;
            break;
        case NX_CRYPTO_EC_SECP384R1:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_384);
            key_type = PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_CURVE_SECP384R1);
            curve_size = 48;
            break;
        default:
            return(NX_CRYTPO_MISSING_ECC_CURVE);
    }

    if (!ecdsa_ptr->key_id)
    {
        nx_status = _nx_crypto_ecdsa_psa_import_key(ecdsa_ptr, alg, key_type, public_key, public_key_length);
        if (nx_status)
        {
            return(nx_status);
        }
        imported_key = 1;
    }

    ecdsa_ptr->alg = alg;

    if (!ecdsa_ptr->key_id || ecdsa_ptr->alg == 0)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    if (signature[1] >= curve_size)
    {
        NX_CRYPTO_MEMCPY(ecdsa_ptr->nx_crypto_ecdsa_scratch, &signature[2 + signature[1] - curve_size], curve_size);
    }
    else
    {
        NX_CRYPTO_MEMSET(ecdsa_ptr->nx_crypto_ecdsa_scratch, 0, curve_size - signature[1]);
        NX_CRYPTO_MEMCPY(&ecdsa_ptr->nx_crypto_ecdsa_scratch[curve_size - signature[1]], &signature[2 + curve_size - signature[1]], signature[1]);
    }
    signature_length -= (signature[1] + 2u);
    signature += signature[1] + 2;

    if (signature_length < (signature[1] + 2u))
    {
        return(NX_CRYPTO_SIZE_ERROR);
    }

    if (signature[1] >= curve_size)
    {
        NX_CRYPTO_MEMCPY(&ecdsa_ptr->nx_crypto_ecdsa_scratch[curve_size], &signature[2 + signature[1] - curve_size], curve_size);
    }
    else
    {
        NX_CRYPTO_MEMSET(&ecdsa_ptr->nx_crypto_ecdsa_scratch[curve_size], 0, curve_size - signature[1]);
        NX_CRYPTO_MEMCPY(&ecdsa_ptr->nx_crypto_ecdsa_scratch[(curve_size << 1) - signature[1]], &signature[2 + curve_size - signature[1]], signature[1]);
    }

    psa_status = psa_open_key(ecdsa_ptr -> key_id, &key_handle);
    if (psa_status != PSA_SUCCESS)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    /* Verify */
    psa_status = psa_asymmetric_verify(key_handle,
                                       ecdsa_ptr->alg,
                                       hash,
                                       hash_length,
                                       (const uint8_t *)ecdsa_ptr->nx_crypto_ecdsa_scratch,
                                       curve_size << 1);

    if (psa_status != PSA_SUCCESS)
    {
        return(NX_CRYPTO_NOT_SUCCESSFUL);
    }

    /* Destroy imported keys, close persistent keys. */
    if (imported_key)
    {
        psa_status = psa_destroy_key(key_handle);
    }
    else
    {
        psa_status = psa_close_key(key_handle);
    }

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
/*    _nx_crypto_method_ecdsa_psa_crypto_init            PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the common crypto method init callback for         */
/*    Express Logic supported ECDSA cryptograhic algorithm.               */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_ecdsa_psa_crypto_init(struct  NX_CRYPTO_METHOD_STRUCT *method,
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

    /* Verify the metadata address is 4-byte aligned. */
    if ((((ULONG)crypto_metadata) & 0x3) != 0)
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    if (crypto_metadata_size < sizeof(NX_CRYPTO_ECDSA_PSA_CRYPTO))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    NX_CRYPTO_MEMSET(crypto_metadata, 0, sizeof(NX_CRYPTO_ECDSA_PSA_CRYPTO));

    return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_ecdsa_psa_crypto_cleanup          PORTABLE C      */
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
NX_CRYPTO_KEEP UINT  _nx_crypto_method_ecdsa_psa_crypto_cleanup(VOID *crypto_metadata)
{

    NX_CRYPTO_STATE_CHECK

#ifdef NX_SECURE_KEY_CLEAR
    if (!crypto_metadata)
        return (NX_CRYPTO_SUCCESS);

    /* Destroy the key */
    NX_CRYPTO_ECDSA_PSA_CRYPTO *ecdsa = (NX_CRYPTO_ECDSA_PSA_CRYPTO *)crypto_metadata;
    psa_destroy_key(ecdsa->key_handle);

    /* Clean up the crypto metadata.  */
    NX_CRYPTO_MEMSET(crypto_metadata, 0, sizeof(NX_CRYPTO_ECDSA_PSA_CRYPTO));
#else
    NX_CRYPTO_PARAMETER_NOT_USED(crypto_metadata);
#endif/* NX_SECURE_KEY_CLEAR  */

    return(NX_CRYPTO_SUCCESS);
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_ecdsa_psa_crypto_operation       PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs an ECDSA operation.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    op                                    ECDSA operation               */
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
/*    _nx_crypto_ecdsa_sign_psa_crypto     Sign using ECDSA              */
/*    _nx_crypto_ecdsa_verify_psa_crypto   Verify ECDSA signature        */
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
NX_CRYPTO_KEEP UINT _nx_crypto_method_ecdsa_psa_crypto_operation(UINT op,
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
NX_CRYPTO_ECDSA_PSA_CRYPTO *ecdsa;
UINT                        status = NX_CRYPTO_SUCCESS;
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

    if (crypto_metadata_size < sizeof(NX_CRYPTO_ECDSA_PSA_CRYPTO))
    {
        return(NX_CRYPTO_PTR_ERROR);
    }

    ecdsa = (NX_CRYPTO_ECDSA_PSA_CRYPTO *)crypto_metadata;


    if (op == NX_CRYPTO_EC_CURVE_SET)
    {
        /* Set curve to be used in the ECDSA. */
        status = ((NX_CRYPTO_METHOD *)input) -> nx_crypto_operation(NX_CRYPTO_EC_CURVE_GET,
                                                                    NX_CRYPTO_NULL,
                                                                    (NX_CRYPTO_METHOD *)input,
                                                                    NX_CRYPTO_NULL, 0,
                                                                    NX_CRYPTO_NULL, 0,
                                                                    NX_CRYPTO_NULL,
                                                                    (UCHAR *)&ecdsa -> nx_crypto_ecdsa_curve,
                                                                    sizeof(NX_CRYPTO_EC *),
                                                                    NX_CRYPTO_NULL, 0,
                                                                    NX_CRYPTO_NULL, NX_CRYPTO_NULL);
        /* TODO: use PSA crypto API to set the curve */
    }
    else if (op == NX_CRYPTO_AUTHENTICATE)
    {
        if (key == NX_CRYPTO_NULL)
        {
            return(NX_CRYPTO_PTR_ERROR);
        }

        extended_output = (NX_CRYPTO_EXTENDED_OUTPUT *)output;

        status = _nx_crypto_ecdsa_sign_psa_crypto(ecdsa,
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

        status = _nx_crypto_ecdsa_verify_psa_crypto(ecdsa,
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
