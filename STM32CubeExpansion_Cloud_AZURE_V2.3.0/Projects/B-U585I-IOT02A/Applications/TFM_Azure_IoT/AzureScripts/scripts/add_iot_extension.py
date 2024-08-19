# ******************************************************************************
# * @file    add-iot-extension.py
# * @author  MCD Application Team
# * @brief   add Azure IOT extension
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

from azure.cli.core import get_default_cli

ADD_AZURE_IOT_EXTENSION            =    'extension add --name azure-iot'


#get default Azure CLI
az_cli = get_default_cli()

#Run the iot extension cmd
az_cli.invoke(ADD_AZURE_IOT_EXTENSION.split(' '))