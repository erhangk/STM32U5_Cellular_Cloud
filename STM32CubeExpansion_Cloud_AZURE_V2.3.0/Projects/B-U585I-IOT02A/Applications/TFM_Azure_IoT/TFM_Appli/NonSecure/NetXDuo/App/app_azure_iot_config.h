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
/* The following source code has been taken from 'sample_config.h'                      */
/* by Microsoft (https://github.com/azure-rtos) and modified by STMicroelectronics.     */
/* Main changes summary :                                                               */
/* - Adaptation to STMicroelectronics B-U585I-IoT02A Discovery board PnP Model          */
/* - Add WiFi SSID and password configuration                                           */
/*                                                                                      */
/****************************************************************************************/

#ifndef APP_AZURE_IOT_CONFIG_H
#define APP_AZURE_IOT_CONFIG_H

#ifdef __cplusplus
extern   "C" {
#endif

/*
  Configuration section.
*/

/* Define ENABLE_ADU to enable Device Update.
   This implies using a specific IoT Plug and Play device model required for ADU.
   It must be disabled to have IoT Plug and Play device model for IoT Central.
 */
#define ENABLE_ADU

/* This application uses TF-M crypto drivers for TLS communication. */
/* Comment ENABLE_PSA_CRYPTO_CIPHERSUITES to to use NetXDuo Crypto libraries instead. */
#define ENABLE_PSA_CRYPTO_CIPHERSUITES

/* Define USE_DEVICE_CERTIFICATE and set up your device certificate in device_identity.c
   to connect to IoT Hub with an x509 certificate. Set up X.509 security in your Azure IoT Hub,
   refer to https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-security-x509-get-started  */
/*
*/
#define USE_DEVICE_CERTIFICATE    1

/* This example can also use the pre-loaded STSAFE certificate. If this macro is defined,
   the application will load the DER-encoded certificate from the STSAFE device, then use the
   associated protected private key for TLS operations. */

#define USE_TFM_STSAFE_CERTIFICATE

/* This macro gives an example of X.509 storage using TF-M persistent keys. The example will
   load a user key into the TF-M cryptography service and then use the persistent key handle
   for TLS operations. */
#if (USE_DEVICE_CERTIFICATE == 1) && !defined(USE_TFM_STSAFE_CERTIFICATE)
#define USE_TFM_X509_STORAGE     /* Comment out to disable TF-M X.509 storage. */
#endif

/* ID to use for the persistent X.509 device private key (if using TF-M X.509 persistent keys). */
#define TFM_CRYPTO_X509_KEY_ID    (1)

/* Slot number where STSAFE certificate (DER-encoded) is stored. The default slot is 0. */
#define TFM_STSAFE_X509_SLOT  (0)

/*
To be configured : core settings of application for your IoTHub.
*/

/* Defined, Device Provisioning Service is enabled.  */

#define ENABLE_DPS

#ifndef ENABLE_DPS

/* Required when DPS is not used.  */
/* These values can be picked from device connection string which is of format : HostName=<host1>;DeviceId=<device1>;SharedAccessKey=<key1>
   HOST_NAME is the IoT Hub host name (<host1> in connection string),
   DEVICE_ID can be set to <device1>,
   DEVICE_SYMMETRIC_KEY can be set to <key1>.  */
#ifndef HOST_NAME
#define HOST_NAME                                   ""
#endif /* HOST_NAME */

#ifndef DEVICE_ID
#define DEVICE_ID                                   ""
#endif /* DEVICE_ID */

#else /* !ENABLE_DPS */

#if !defined(GET_CONFIG_FROM_SECURE_STORAGE)
/* Required when DPS is used.  */
#ifndef ENDPOINT
#define ENDPOINT                                    ""
#endif /* ENDPOINT */

#ifndef ID_SCOPE
#define ID_SCOPE                                    ""
#endif /* ID_SCOPE */
#endif /* GET_CONFIG_FROM_SECURE_STORAGE */

#ifndef REGISTRATION_ID
#define REGISTRATION_ID                             ""
#endif /* REGISTRATION_ID */

#endif /* ENABLE_DPS */

/* Optional SYMMETRIC KEY.  */
#ifndef DEVICE_SYMMETRIC_KEY
#define DEVICE_SYMMETRIC_KEY                        ""
#endif /* DEVICE_SYMMETRIC_KEY */

/* Optional module ID.  */
#ifndef MODULE_ID
#define MODULE_ID                                   ""
#endif /* MODULE_ID */

#if (USE_DEVICE_CERTIFICATE == 1)

/* Using X509 certificate authenticate to connect to IoT Hub,
   set the device certificate as your device.  */

/* Device Key type. */
#ifndef DEVICE_KEY_TYPE
#ifdef ENABLE_PSA_CRYPTO_CIPHERSUITES
#define DEVICE_KEY_TYPE                             NX_SECURE_X509_KEY_TYPE_HARDWARE
#else
#define DEVICE_KEY_TYPE                             NX_SECURE_X509_KEY_TYPE_RSA_PKCS1_DER
#endif
#endif /* DEVICE_KEY_TYPE */

#endif /* USE_DEVICE_CERTIFICATE */

#if !defined(GET_CONFIG_FROM_SECURE_STORAGE)
#define WIFI_SSID                                   ""
#define WIFI_PASSWORD                               ""
#else
extern char * pWIFI_SSID;
extern char * pWIFI_PASSWORD;
#define WIFI_SSID                                   (pWIFI_SSID)
#define WIFI_PASSWORD                               (pWIFI_PASSWORD)

extern char * pENDPOINT;
extern char * pID_SCOPE;
#define ENDPOINT                                    (pENDPOINT)
#define ID_SCOPE                                    (pID_SCOPE)
#endif /* GET_CONFIG_FROM_SECURE_STORAGE */

/*
END To configure section
*/

/* Define the Azure Iot Plug and Play device template */

#ifdef ENABLE_ADU
/* Azure IoT Plug and Play model for ADU */
#define NX_AZURE_IOT_PNP_MODEL_ID                                             "dtmi:azure:iot:deviceUpdateModel;1"
#else
/* Azure IoT Plug and Play model for IoT Central */
#define NX_AZURE_IOT_PNP_MODEL_ID                                             "dtmi:stmicroelectronics:b_u585i_iot02a:standard_fw;3"
#endif /*  ENABLE_ADU */

/* Define the Azure RTOS IOT thread stack and priority.  */
#ifndef NX_AZURE_IOT_STACK_SIZE
#define NX_AZURE_IOT_STACK_SIZE                     (2560)
#endif /* NX_AZURE_IOT_STACK_SIZE */

#ifndef NX_AZURE_IOT_THREAD_PRIORITY
#define NX_AZURE_IOT_THREAD_PRIORITY                (4)
#endif /* NX_AZURE_IOT_THREAD_PRIORITY */

/* Define the Azure IOT max size of device id and hostname buffers */
#ifndef NX_AZURE_IOT_MAX_BUFFER
#define NX_AZURE_IOT_MAX_BUFFER                    (256)
#endif /* NX_AZURE_IOT_MAX_BUFFER */

#ifdef __cplusplus
}
#endif
#endif /* APP_AZURE_IOT_CONFIG_H */
