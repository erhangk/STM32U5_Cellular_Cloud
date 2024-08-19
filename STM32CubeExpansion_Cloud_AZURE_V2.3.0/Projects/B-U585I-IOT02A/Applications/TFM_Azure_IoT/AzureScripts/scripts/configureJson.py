# ******************************************************************************
# * @file    configureJson.py
# * @author  MCD Application Team
# * @brief   Configure the Config.json file
# ******************************************************************************
# * @attention
# *
# * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
# * All rights reserved.</center></h2>
# *
# * This software component is licensed by ST under BSD 3-Clause license,
# * the "License"; You may not use this file except in compliance with the
# * License. You may obtain a copy of the License at:
# *                        opensource.org/licenses/BSD-3-Clause
# *
# ******************************************************************************
import json
import random
import string

from uuid import getnode as get_mac

CONFIG_PATH     = '.\\Config.json'
APP_NAME        = 'vws'
SSID            = 'st_iot_demo'
PSWD            = 'stm32u585'
RESOURCE_GROUP  = 'vws-rg'
LOCATION        = 'westus'
DPS             = 'vws-dps'
IOTHUB          = 'vws-IotHub'
ADUGROUP        = ''
STORAGEACCOUNT  = 'vwsstorage'
ADUACCOUNT      = 'vws-adu'
ADUINSTANCE     = 'vws-adu'
CONTAINER       = ''


# Load credentials from the CONFIG_PATH into credentials dictionary
def loadCredentials(CONFIG_PATH):
    configFile = open(CONFIG_PATH)
    credentials = json.load(configFile)
    configFile.close()
    return credentials

# Update Credentials File
def writeCredentialsToFile(credentials, CONFIG_PATH):
    with open(CONFIG_PATH, 'w') as outfile:
        json.dump(credentials, outfile, indent=4)

def main():
    mac = get_mac()
    credentials = loadCredentials(CONFIG_PATH)

    # initializing size of string
    N = 12

    credentials['Entered']['AppName'] = APP_NAME + '-'+ hex(mac)[-5:-1]+ ''.join(random.choices(string.ascii_lowercase + string.digits, k=N))
    credentials['Entered']['SSID'] = SSID
    credentials['Entered']['Password'] = PSWD
    credentials['Entered']['ResourceGroup'] = RESOURCE_GROUP
    credentials['Entered']['Location'] = LOCATION
    credentials['Entered']['DPS'] = DPS
    credentials['Entered']['IotHub'] = IOTHUB

    credentials['Generated']['AppID'] = str('')
    credentials['Generated']['Cert'] = str('')
    credentials['Generated']['RegistrationID'] = str('')
    credentials['Generated']['IDScope'] = str('')

    credentials['ADU']['Group'] = ADUGROUP
    credentials['ADU']['StorageAccount'] = STORAGEACCOUNT
    credentials['ADU']['Account'] = ADUACCOUNT
    credentials['ADU']['Instance'] = ADUINSTANCE
    credentials['ADU']['Container'] = CONTAINER



    writeCredentialsToFile(credentials, CONFIG_PATH)

if __name__ == "__main__":
    main()