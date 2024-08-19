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

/****************************************************************************************/
/*                                                                                      */
/* The following source code has been taken from 'sample_device_identity.c'             */
/* by Microsoft (https://github.com/azure-rtos) and modified by STMicroelectronics.     */
/* Main changes summary :                                                               */
/* - Modified to add an API instead of using extern variable declaration                */
/*                                                                                      */
/****************************************************************************************/

#include "nx_api.h"
#include "app_azure_iot_config.h"

#if defined(USE_TFM_X509_STORAGE) || defined(USE_TFM_STSAFE_CERTIFICATE)
#include "se_psa_id.h"
#include "nx_azure_iot_ciphersuites.h"
#endif

#ifndef USE_TFM_STSAFE_CERTIFICATE

/* Device certificate.  */
#ifndef DEVICE_CERT
#define DEVICE_CERT {0x00}
#endif /* DEVICE_CERT */

/* Device Private Key.  */
#ifndef DEVICE_PRIVATE_KEY
#define DEVICE_PRIVATE_KEY {0x00}
#endif /* DEVICE_PRIVATE_KEY */

static const UCHAR device_cert_ptr[] = DEVICE_CERT;
static const UINT device_cert_len = sizeof(device_cert_ptr);
static const UCHAR device_private_key_ptr[] = DEVICE_PRIVATE_KEY;
static const UINT device_private_key_len = sizeof(device_private_key_ptr);

#else

#define CERT_HEAD           "-----BEGIN CERTIFICATE-----\n"
#define CERT_TAIL           "-----END CERTIFICATE-----\n"
#define CERT_HEAD_LENGTH    (sizeof(CERT_HEAD) - 1)
#define CERT_TAIL_LENGTH    (sizeof(CERT_TAIL) - 1)

static uint32_t psa_cert_id;
static psa_key_handle_t psa_key_handle;

/* Storage for the STSAFE certificate extracted for use in TLS. */
static uint8_t stsafe_cert[2000];
static size_t  stsafe_cert_length = sizeof(stsafe_cert);
static UCHAR   pem_data[2000];
static UINT    pem_size = sizeof(pem_data); // Size of data in the output PEM buffer (a text string)
static UINT    byte_count = 0;
static UINT    totalSize = 0;

static int32_t print_pem_data(uint8_t *data, size_t size, size_t line)
{
    int32_t n_item_per_line;
    int32_t i, index = 0;
    int32_t total_size = size;

    while (index < size)
    {
        if (line != 0)
        {
            n_item_per_line = (size - index) >= line ? line : (size - index);
        }
        else
        {
            n_item_per_line = size;
        }
        for (i = 0; i < n_item_per_line; i++)
        {
            printf("%c", data[index + i]);
        }
        printf("\r\n");
        index += n_item_per_line;
        total_size += 2;
    }

    return total_size;
}

#endif /* !(USE_TFM_STSAFE_CERTIFICATE) */

#if defined(USE_TFM_X509_STORAGE) || defined(USE_TFM_STSAFE_CERTIFICATE)
static uint32_t psa_key_id;
psa_status_t psa_x509_private_key_store(uint32_t key_id, const void *private_key, uint32_t key_length);
#endif

UINT device_identity_retrieve_credentials(const UCHAR  **cert_ptr, UINT *cert_len, const UCHAR **private_key_ptr, UINT *private_key_len) 
{
  UINT status;
  if(cert_ptr != NULL && cert_len != NULL && private_key_ptr != NULL && private_key_len != NULL) 
  {
#ifdef USE_TFM_X509_STORAGE
    psa_status_t psa_status;
    /* Store the private key in TF-M crypto partition as a persistent key using TFM_CRYPTO_X509_KEY_ID as the key identifier. 
        This function may be called to load the private key in a separate application during device provisioning. It is used
        here as an example. */           
    psa_key_id = TFM_CRYPTO_X509_KEY_ID;
    psa_status = psa_x509_private_key_store(psa_key_id, device_private_key_ptr, device_private_key_len);

    if(psa_status != PSA_SUCCESS)
    {
        printf("Failed to store private key to TF-M protected storage. error code = 0x%08x\r\n", psa_status);
        return(NX_SECURE_PKCS1_INVALID_PRIVATE_KEY);
    }
#endif /* USE_TFM_X509_STORAGE */
    
#ifdef USE_TFM_STSAFE_CERTIFICATE
      /* Using STSAFE certificate with TF-M. STSAFE is accessed through TF-M using
        a special key id, and the certificate is stored in an STSAFE slot. Load
        the certificate here for use in TLS. */
      psa_key_id = SE_ST_ID_TO_PSA_ID(SE_ST_PRIV_SLOT_0);     
      psa_cert_id = SE_ST_ID_TO_PSA_ID(SE_ST_MEMORY_REGION_ID(TFM_STSAFE_X509_SLOT));
      
      status = psa_open_key(psa_cert_id, &psa_key_handle);
      if (status != PSA_SUCCESS)
      {
          printf("psa_open_key failed error =%d\n\r", status);
          return(NX_SECURE_X509_INVALID_CERTIFICATE_DATA);
      }
      uint8_t cert_header[4];
      
      status = psa_export_key(psa_key_handle, cert_header, 4, &stsafe_cert_length);
      if (status != PSA_SUCCESS)
      {
          printf("psa_export_key failed error =%d\n\r", status);
          return(NX_SECURE_X509_INVALID_CERTIFICATE_DATA);
      }

      stsafe_cert_length = ( (cert_header[2] << 8) | cert_header[3] ) + 4;

      status = psa_export_key(psa_key_handle, stsafe_cert, stsafe_cert_length, &stsafe_cert_length);
      if (status != PSA_SUCCESS)
      {
          printf("psa_export_key failed error =%d\n\r", status);
          return(NX_SECURE_X509_INVALID_CERTIFICATE_DATA);
      }
      status = psa_close_key(psa_key_handle);
      if (status != PSA_SUCCESS)
      {
          printf("psa_close_key failed error =%d\n\r", status);
          return(NX_SECURE_X509_INVALID_CERTIFICATE_DATA);
      }

      /* Convert cert to PEM format */
      status = _nx_utility_base64_encode(stsafe_cert, stsafe_cert_length, pem_data, pem_size, &byte_count);

      if (status != NX_SUCCESS)
      {
          printf("_nx_utility_bas64_encode failed error =%d\n\r", status);
          return(NX_SECURE_X509_INVALID_CERTIFICATE_DATA);
      }

      /* Print PEM CERT */
      printf("\n\rCertificate in PEM format :\n\r");
      printf("\033[0;32m\n\r"); // Change to green color
      printf("%s\r", CERT_HEAD);
      totalSize = print_pem_data(pem_data, byte_count, 64);
      printf("%s\r\n\r", CERT_TAIL);
      printf("\033[0;39;49m"); // Revert back to black and white
      printf("\n\rPEM Certificate size = %i\n\r\n\r", totalSize + CERT_HEAD_LENGTH + CERT_TAIL_LENGTH);
#endif /* USE_TFM_STSAFE_CERTIFICATE */

#if defined(USE_TFM_STSAFE_CERTIFICATE)
      *cert_ptr =  (UCHAR *)stsafe_cert;
      *cert_len = (USHORT)stsafe_cert_length;
#else
      *cert_ptr = device_cert_ptr;
      *cert_len = device_cert_len;
#endif

#if defined(USE_TFM_X509_STORAGE) || defined(USE_TFM_STSAFE_CERTIFICATE)
      *private_key_ptr = (UCHAR *)&psa_key_id;
      *private_key_len = sizeof(psa_key_id);
#else
      *private_key_ptr = device_private_key_ptr;
      *private_key_len = device_private_key_len;
#endif
  }
  else
  {
    status = NX_INVALID_PARAMETERS;
  }

  return status;
}

