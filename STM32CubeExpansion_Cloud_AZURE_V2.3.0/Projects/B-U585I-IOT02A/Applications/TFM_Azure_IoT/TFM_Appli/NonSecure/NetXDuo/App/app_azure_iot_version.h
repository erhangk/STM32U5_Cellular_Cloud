/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_azure_iot_version.h
  * @author  GPM Application Team
  * @brief   Firmware version definition.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the ST_LICENSE file
  * in the root directory of this software component.
  * If no ST_LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef app_azure_iot_version_H
#define app_azure_iot_version_H
#include "nx_user.h"

/* Device properties.  */
#ifndef APP_DEVICE_MANUFACTURER
#define APP_DEVICE_MANUFACTURER "STMicroelectronics"
#endif /* APP_DEVICE_MANUFACTURER*/

#ifndef APP_DEVICE_MODEL
#define APP_DEVICE_MODEL "B-U585I-IOT02A"
#endif /* APP_DEVICE_MODEL */


#if (NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT >= 1)
/* Device properties.  */
#ifndef APP_LEAF_DEVICE_MANUFACTURER
#define APP_LEAF_DEVICE_MANUFACTURER APP_DEVICE_MANUFACTURER
#endif /* APP_LEAF_DEVICE_MANUFACTURER*/

#ifndef APP_LEAF_DEVICE_MODEL
#define APP_LEAF_DEVICE_MODEL APP_DEVICE_MODEL"-S"
#endif /* APP_LEAF_DEVICE_MODEL */

#endif /* NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT */


#define FIRMWARE_NS_NAME        "TFM_Azure_IoT"

#define FIRMWARE_S_NAME         "TFM_Secure_fw"

//#define FW_VERSION_MAJOR 2    // Retrieved from TFM_PSA
//#define FW_VERSION_MINOR 2    // Retrieved from TFM_PSA
//#define FW_VERSION_PATCH 0    // Retrieved from TFM_PSA
#define PKG_DATE "14-April-2023 03:00:00 PM"
#define PKG_NAME  "X-CUBE-AZURE"

#endif /* app_azure_iot_version_H */
