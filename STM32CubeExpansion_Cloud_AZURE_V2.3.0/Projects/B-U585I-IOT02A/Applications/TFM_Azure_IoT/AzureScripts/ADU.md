% Azure Device Update on B-U585I-IOT02A

The Azure Device Update example in TFM_Azure_IoT project on B-U585I-IOT02A
allows to update either the non-secure application or the TFM secure application, or both at same time.

## Prerequisites

- You must have an Azure subscription and be able to log into [Azure web portal](https://portal.azure.com)

- Log into the Azure web portal

- If you don't already have an IoT Hub, create an IoT hub resource. It must be of "Tier S" to support Device Update service.

  > Note: to create a resource, use the Azure web portal menu on the top left side and click on "create resource"

- In the IoT hub, you must have a device created. See main README of TFM_Azure_IoT application to know how to create it.

  The devices must have a tag named "ADUGroup" to be recognized by Update service.

  Create the tag "ADUGroup" in the device twin and put a value (ex: "STM32U5"). This will be used during Update deployment.

      "tags": {
        "ADUGroup": "<CustomTagValue>"
      }

- It is needed to have a "Device Update for IoT hub" resource with an instance linked to your IoT Hub. To create it, follow the instructions at [Azure documentation reference page](https://docs.microsoft.com/azure/iot-hub-device-update/create-device-update-account).

- It is needed to have an Azure storage account with a container created.

## Prepare the binary files

- Compile the TFM_Azure_IoT project and program the B-U585I-IOT02A board with it.
  See main README for that.

- To create a new version of firmware binary files, the version number
  must be incremented in the project source, and the projects must be rebuilt.

  - Update the VERSION files in the secure and non-secure applications  

  </br>

  After changing the version number, build the projects.  
  The output binary files are under `Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Appli/Binary` :
  
    Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Appli/Binary/tfm_ns_app_enc_sign.bin  
    Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Appli/Binary/tfm_s_app_enc_sign.bin

  Copy these files to `Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/AzureDeviceUpdateScripts`.


## Create the import manifest

An import manifest is a JSON file that describes to Azure the files to be imported to create an Update.
It contains the file name, version, hash number, and installation criteria.

- Open a command prompt (CMD.exe) or a shell prompt in directory `Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/AzureDeviceUpdateScripts`  

- run the script STM32U5_ADU_CreateUpdatePackage.bat or STM32U5_ADU_CreateUpdatePackage.sh, depending of your environment, providing the newly generated binary file(s) to import.  
  run `STM32U5_ADU_CreateUpdatePackage.(bat|sh) -h` for a detailed parameters description.  

  Example:

      STM32U5_ADU_CreateUpdatePackage.(bat|sh) -ns tfm_ns_app_init.bin -s tfm_s_app_init.bin

  it is possible to create an import manifest for one of the non-secure application binary
  or the secure binary, or both. The script creates 1 or 2 import manifest JSON files
  depending if you specify the secure and/or the non-secure application.

- In Azure web portal, go to your Azure storage container. Upload the binary files and the import manifest(s) JSON files.

## Import the update in Azure cloud


- Go to your IoT Hub resource and click on Updates.

- In Updates, click on Updates then "Import a new update".

- Click on "Select from storage container". Find your storage container
  and select the JSON import file(s) and the binary files that you have uploaded previously.

- wait until the import is complete (you can click on "View Import history" and Refresh to see the progress).


## Deploy the update

- make sure your device has an "ADUGroup" tag in its device twin,
  and it has already connected to the IoT Hub.

- In the IoT Hub Updates section, click on "Groups and deployments".

- If the device has already connected to the IoT Hub with its device twin tag "ADUGroup",
  the group name should appear in "Groups and deployments". Click on the group name. It should contain the device.

- The imported update should appear as new available update for this group. 
  Click on Deploy button. Select "Start immediately" and click on Create.

- on the device's virtual com port, the update process will output some log traces.

- after downloading the new firmware(s), the device will reboot to install the new versions.

- the new versions should appear in device's serial output.

## Troubleshooting

- if the group name does not appear in "Groups and deployments", make sure your device has an "ADUGroup" tag in its device twin,
  and it has already connected to the IoT Hub. Check the value of the group name in "ADUGroup" tag.

- try to click on Refresh buttons of IoT Hub Update page to update contents.

## Reference

[Device Update Azure RTOS](https://learn.microsoft.com/en-us/azure/iot-hub-device-update/device-update-azure-real-time-operating-system)
