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

#include "nx_azure_iot_adu_agent.h"
#include "psa/update.h"
#include "tfm_platform_api.h"
#include "platform_ioctl.h"

/* ADU driver for secure image.  */
void nx_azure_iot_adu_agent_proxy_driver(NX_AZURE_IOT_ADU_AGENT_DRIVER *driver_req_ptr);

static psa_image_id_t image_id_s = \
                (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_STAGE,
                                                       FWU_IMAGE_TYPE_SECURE,
                                                       0);

static psa_image_id_t image_id_s_running = \
                (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_ACTIVE,
                                                       FWU_IMAGE_TYPE_SECURE,
                                                       0);

/* Define the flash unit size.  */
#define FLASH0_PROG_UNIT 0x10

static UINT firmware_size_total = 0;
static UINT firmware_size_count = 0;
static UCHAR write_buffer[FLASH0_PROG_UNIT];
static UINT write_buffer_count = 0;
static INT internal_flash_write(psa_image_id_t image_id, UCHAR *data_ptr, UINT data_size, UINT data_offset);
INT internal_version_compare(psa_image_id_t image_id, const UCHAR *buffer_ptr, UINT buffer_len);

/****** DRIVER SPECIFIC ******/
void nx_azure_iot_adu_agent_proxy_driver(NX_AZURE_IOT_ADU_AGENT_DRIVER *driver_req_ptr)
{
psa_image_id_t dependency_uuid;
psa_image_version_t dependency_version;
psa_image_info_t info;
psa_status_t status;

    /* Default to successful return.  */
    driver_req_ptr -> nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_SUCCESS;
        
    /* Process according to the driver request type.  */
    switch (driver_req_ptr -> nx_azure_iot_adu_agent_driver_command)
    {

        case NX_AZURE_IOT_ADU_AGENT_DRIVER_INITIALIZE:
        {

            /* Process initialize requests.  */
            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_DRIVER_UPDATE_CHECK:
        {

            /* Read the version of secure image and compare the version in update_id to check if the update is installed or not.
               If installed, return NX_TRUE, else return NX_FALSE.  */
            if (internal_version_compare(image_id_s_running, 
                                         driver_req_ptr -> nx_azure_iot_adu_agent_driver_installed_criteria, 
                                         driver_req_ptr -> nx_azure_iot_adu_agent_driver_installed_criteria_length) <= 0)
            {
                *(driver_req_ptr -> nx_azure_iot_adu_agent_driver_return_ptr) = NX_TRUE;
            }
            else
            {
                *(driver_req_ptr -> nx_azure_iot_adu_agent_driver_return_ptr) = NX_FALSE;
            }
            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_DRIVER_PREPROCESS:
        {

            /* Process firmware preprocess requests before writing firmware.
               Such as: erase the flash at once to improve the speed.  */

            /* Abort the previous update if exists. */
            psa_fwu_abort(image_id_s);
            firmware_size_total = driver_req_ptr -> nx_azure_iot_adu_agent_driver_firmware_size;
            firmware_size_count = 0;
            write_buffer_count = 0;

            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_DRIVER_WRITE:
        {
        
            /* Process firmware write requests.  */
            
            /* Write firmware contents.
               1. This function must be able to figure out which bank it should write to.
               2. Write firmware contents into new bank.
               3. Decrypt and authenticate the firmware itself if needed.
            */
            status = internal_flash_write(image_id_s,
                                          driver_req_ptr -> nx_azure_iot_adu_agent_driver_firmware_data_ptr,
                                          driver_req_ptr -> nx_azure_iot_adu_agent_driver_firmware_data_size,
                                          driver_req_ptr -> nx_azure_iot_adu_agent_driver_firmware_data_offset);
            if (status)
            {
                driver_req_ptr -> nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_FAILURE;
            }

            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_DRIVER_INSTALL:
        {

            if (firmware_size_count < firmware_size_total)
            {
                driver_req_ptr -> nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_FAILURE;
                break;
            }

            /* Set the new firmware for next boot.  */
            status = psa_fwu_install(image_id_s, &dependency_uuid, &dependency_version);

            /* In the current implementation, image verification is deferred to
             * reboot, so PSA_SUCCESS_REBOOT is returned when success.
             */
            if ((status != PSA_SUCCESS_REBOOT) &&
                (status != PSA_ERROR_DEPENDENCY_NEEDED))
            {
                driver_req_ptr -> nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_FAILURE;
                break;
            }

            /* Query the staging image. */
            if (psa_fwu_query(image_id_s, &info) != PSA_SUCCESS)
            {
                driver_req_ptr -> nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_FAILURE;
                break;
            }

            /* Check the image state. */
            if ((status == PSA_SUCCESS_REBOOT) &&
                (info.state != PSA_IMAGE_REBOOT_NEEDED))
            {
                driver_req_ptr -> nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_FAILURE;
            }

            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_DRIVER_APPLY:
        {

            /* Apply the new firmware, and reboot device from that.*/
            psa_fwu_request_reboot();

            break;
        }
        default:
        {

            /* Invalid driver request.  */

            /* Default to successful return.  */
            driver_req_ptr -> nx_azure_iot_adu_agent_driver_status =  NX_AZURE_IOT_FAILURE;
        }
    }
}

INT internal_version_compare(psa_image_id_t image_id, const UCHAR *buffer_ptr, UINT buffer_len)
{
psa_image_info_t info;
psa_status_t status;
UINT i, j, pos;
UINT version[4] = {0};

    /* Get the version of the image.  */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_SUCCESS)
    {
        return(-2);
    }

    for (i = 0, j = 0, pos = 0; i <= buffer_len; i++)
    {
        if ((buffer_ptr[i] == '.') || (i == buffer_len))
        {
            status = _nx_utility_string_to_uint((CHAR *)&buffer_ptr[pos], i - pos, &version[j]);
            if (status)
            {
                return(-2);
            }
            pos = i + 1;
            j++;
        }
    }

    /* Compare the version.  */
    if (version[0] > info.version.iv_major)
    {
        return(1);
    }
    if (version[0] < info.version.iv_major)
    {
        return(-1);
    }
    if (version[1] > info.version.iv_minor)
    {
        return(1);
    }
    if (version[1] < info.version.iv_minor)
    {
        return(-1);
    }
    if (version[2] > info.version.iv_revision)
    {
        return(1);
    }
    if (version[2] < info.version.iv_revision)
    {
        return(-1);
    }
    return 0;
}

static INT internal_flash_write(psa_image_id_t image_id, UCHAR *data_ptr, UINT data_size, UINT data_offset)
{
UINT status;
UINT remaining_size = data_size;
UINT block_size = 0;

    if (firmware_size_count + data_size + write_buffer_count > firmware_size_total)
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    while ((write_buffer_count > 0) && (write_buffer_count < FLASH0_PROG_UNIT) && (remaining_size > 0))
    {
        write_buffer[write_buffer_count++] = *data_ptr++;
        data_offset++;
        remaining_size--;
    }

    if (write_buffer_count == FLASH0_PROG_UNIT)
    {
        status = psa_fwu_write(image_id, data_offset - FLASH0_PROG_UNIT, write_buffer, write_buffer_count);
        if (status != PSA_SUCCESS)
        {
            return(status);
        }
        firmware_size_count += write_buffer_count;
        write_buffer_count = 0;
    }

    while (remaining_size > FLASH0_PROG_UNIT)
    {
        if (remaining_size > PSA_FWU_MAX_BLOCK_SIZE)
        {
            block_size = PSA_FWU_MAX_BLOCK_SIZE;
        }
        else
        {
            block_size = remaining_size - (remaining_size % FLASH0_PROG_UNIT);
        }
        status = psa_fwu_write(image_id, data_offset, data_ptr, block_size);
        if (status != PSA_SUCCESS)
        {
            return(status);
        }
        remaining_size -= block_size;
        data_ptr += block_size;
        data_offset += block_size;
        firmware_size_count += block_size;
    }

    if (remaining_size > 0)
    {
        memcpy(&write_buffer[write_buffer_count], data_ptr, remaining_size);
        write_buffer_count += remaining_size;
    }

    if ((write_buffer_count > 0) && (firmware_size_count + write_buffer_count >= firmware_size_total))
    {
        while (write_buffer_count < FLASH0_PROG_UNIT)
        {
            write_buffer[write_buffer_count++] = 0xFF;
        }

        status = psa_fwu_write(image_id, data_offset, write_buffer, write_buffer_count);
        if (status != PSA_SUCCESS)
        {
            return(status);
        }

        data_offset += write_buffer_count;
        firmware_size_count += write_buffer_count;
    }

    return(NX_SUCCESS);
}