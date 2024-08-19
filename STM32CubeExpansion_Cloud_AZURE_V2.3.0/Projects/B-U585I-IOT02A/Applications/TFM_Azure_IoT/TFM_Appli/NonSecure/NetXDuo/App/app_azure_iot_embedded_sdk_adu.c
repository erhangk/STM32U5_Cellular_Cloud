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

/*******************************************************************************************/
/*                                                                                         */
/* The following source code was taken from 'sample_azure_iot_embedded_sdk_adu.c'          */
/* by Microsoft (https://github.com/azure-rtos/netxduo) and modified by STMicroelectronics.*/
/* Main changes summary :                                                                  */
/* - Get versions with psa_fwu_query()                                                     */
/*                                                                                         */
/*******************************************************************************************/

#include "nx_azure_iot_adu_agent.h"
#include "psa/update.h"
#include "app_azure_iot_version.h"


/* Device properties.  */
#ifndef APP_DEVICE_MANUFACTURER
#error Please define APP_DEVICE_MANUFACTURER
#endif /* APP_DEVICE_MANUFACTURER*/

#ifndef APP_DEVICE_MODEL
#error Please define APP_DEVICE_MODEL
#endif /* APP_DEVICE_MODEL */

#if (NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT >= 1)
/* Device properties.  */
#ifndef APP_LEAF_DEVICE_MANUFACTURER
#error Please define APP_LEAF_DEVICE_MANUFACTURER
#endif /* APP_LEAF_DEVICE_MANUFACTURER*/

#ifndef APP_LEAF_DEVICE_MODEL
#error Please define APP_LEAF_DEVICE_MODEL
#endif /* APP_LEAF_DEVICE_MODEL */

#endif /* NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT */

static NX_AZURE_IOT_ADU_AGENT adu_agent;
static UINT adu_agent_started = NX_FALSE;
static char installed_criteria[64];
#if (NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT >= 1)
static char installed_criteria_leaf[64];
#endif

VOID app_adu_start(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr);

extern void nx_azure_iot_adu_agent_driver(NX_AZURE_IOT_ADU_AGENT_DRIVER *driver_req_ptr);
#if (NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT >= 1)
extern void nx_azure_iot_adu_agent_proxy_driver(NX_AZURE_IOT_ADU_AGENT_DRIVER *driver_req_ptr);
#endif /* NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT */

static void adu_agent_update_notify(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
        UINT update_state,
        UCHAR *provider, UINT provider_length,
        UCHAR *name, UINT name_length,
        UCHAR *version, UINT version_length)
{

    if (update_state == NX_AZURE_IOT_ADU_AGENT_UPDATE_RECEIVED)
    {

        /* Received new update.  */
        printf("Received new update: Provider: %.*s; Name: %.*s, Version: %.*s\r\n",
                provider_length, provider, name_length, name, version_length, version);

        /* Start to download and install update immediately for testing.  */
        nx_azure_iot_adu_agent_update_download_and_install(adu_agent_ptr);
    }
    else if(update_state == NX_AZURE_IOT_ADU_AGENT_UPDATE_INSTALLED)
    {

        /* Start to apply update immediately for testing.  */
        nx_azure_iot_adu_agent_update_apply(adu_agent_ptr);
    }
}

VOID app_adu_start(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr)
{
    psa_image_info_t info_s;
    psa_image_info_t info_ns;
    psa_status_t psa_status;
    if (adu_agent_started == NX_FALSE)
    {
        /* Get the non secure image versions.  */
        psa_status = psa_fwu_query(FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_ACTIVE, FWU_IMAGE_TYPE_NONSECURE, 0), &info_ns);
        (void) (psa_status);
        memset(installed_criteria, 0, sizeof(installed_criteria));
        snprintf(installed_criteria, sizeof(installed_criteria) - 1, "%d.%d.%d",
                info_ns.version.iv_major,
                info_ns.version.iv_minor,
                info_ns.version.iv_revision);

        /* Start ADU agent.  */
        if (nx_azure_iot_adu_agent_start(&adu_agent, hub_client_ptr,
                (const UCHAR *)APP_DEVICE_MANUFACTURER, sizeof(APP_DEVICE_MANUFACTURER) - 1,
                (const UCHAR *)APP_DEVICE_MODEL, sizeof(APP_DEVICE_MODEL) - 1,
                (const UCHAR *)"", 0,
                adu_agent_update_notify,
                nx_azure_iot_adu_agent_driver))
        {
            printf("Failed on nx_azure_iot_adu_agent_start!\r\n");
            return;
        }

        printf("Manufacturer: %s, Model: %s, Installed Criteria: %s.\r\n", APP_DEVICE_MANUFACTURER, APP_DEVICE_MODEL, installed_criteria);

#if (NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT >= 1)
        /* Get the secure image versions.  */
        psa_status = psa_fwu_query(FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_ACTIVE, FWU_IMAGE_TYPE_SECURE, 0), &info_s);
        (void)(psa_status);
        memset(installed_criteria_leaf, 0, sizeof(installed_criteria_leaf));
        snprintf(installed_criteria_leaf, sizeof(installed_criteria_leaf) - 1, "%d.%d.%d",
                info_s.version.iv_major,
                info_s.version.iv_minor,
                info_s.version.iv_revision);


        /* Enable proxy update for leaf device.  */
        if (nx_azure_iot_adu_agent_proxy_update_add(&adu_agent,
                (const UCHAR *)APP_LEAF_DEVICE_MANUFACTURER, sizeof(APP_LEAF_DEVICE_MANUFACTURER) - 1,
                (const UCHAR *)APP_LEAF_DEVICE_MODEL, sizeof(APP_LEAF_DEVICE_MODEL) - 1,
                (const UCHAR *)"", 0,
                nx_azure_iot_adu_agent_proxy_driver))
        {
            printf("Failed on nx_azure_iot_adu_agent_proxy_update_add!\r\n");
            return;
        }

        printf("Manufacturer: %s, Model: %s, Installed Criteria: %s.\r\n", APP_LEAF_DEVICE_MANUFACTURER, APP_LEAF_DEVICE_MODEL, installed_criteria_leaf);
#endif /* NX_AZURE_IOT_ADU_AGENT_PROXY_UPDATE_COUNT */

        adu_agent_started = NX_TRUE;
    }
}
