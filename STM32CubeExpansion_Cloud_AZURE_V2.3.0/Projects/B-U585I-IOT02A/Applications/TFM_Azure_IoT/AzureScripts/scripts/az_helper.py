# ******************************************************************************
# * @file    AZ_helper.py
# * @author  MCD Application Team
# * @brief   Communicate with Azure Command Line Interface
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
import sys
import logging
from azure.cli.core import get_default_cli
import webbrowser

REVISION = '1.4.0'


IOT_CENTRAL_APP_URL                 =   'https://APP_NAME.azureiotcentral.com'
LOGIN_CMD                           =   'login'


RESOURCE_GROUP_CHECK_CMD            =    'group,exists,--name,RESOURCE_GROUP'
RESOURCE_GROUP_CMD                  =    'group,create,--name,RESOURCE_GROUP,--location,LOCATION'
IOT_HUB_CMD                         =    'iot,hub,create,--name,HUB_NAME,--resource-group,RESOURCE_GROUP,--sku,S1'
CHECK_IOT_HUB_CMD                   =    'iot,hub,list,--resource-group,RESOURCE_GROUP'

CENTRAL_APPLICATION_CHECK_CMD       =    'iot,central,app,list,--resource-group,RESOURCE_GROUP'
CENTRAL_APPLICATION_CMD             =    'iot,central,app,create,--resource-group,RESOURCE_GROUP,--name,APP_NAME,--sku,ST2,--location,LOCATION,--subdomain,APP_NAME,--display-name,APP_NAME,--query,"applicationId",-o,tsv'
DPS_CMD                             =    'iot,dps,create,--name,DPS_NAME,--resource-group,RESOURCE_GROUP'
CHECK_DPS_CMD                       =    'iot,dps,list,--resource-group,RESOURCE_GROUP'
DEVICE_CMD                          =    'iot,central,device,create,--app-id,APP_NAME,--device-id,DEVICE_NAME,--simulated,false'
LINK_HUB_DPS_CHECK_CMD              =    'iot,dps,linked-hub,list,--dps-name,DPS_NAME,--resource-group,RESOURCE_GROUP'
HUB_HOST_NAME_CMD                   =    'HUB_NAME.azure-devices.net'
LINK_HUB_DPS_GET_CNX_STRING_CMD     =    'iot,hub,connection-string,show,--hub-name,HUB_NAME,-o,tsv'
LINK_HUB_DPS_SCRIPT_CMD             =    'iot,dps,linked-hub,create,--dps-name,DPS_NAME,--resource-group,RESOURCE_GROUP,--connection-string,CONNECTION_STRING'
INDIVIDUAL_ENROLLMENT_CHECK_CMD     =    'iot,dps,enrollment,list,--resource-group,RESOURCE_GROUP,--dps-name,DPS_NAME'
INDIVIDUAL_ENROLLMENT_CMD           =    'iot,dps,enrollment,create,-g,RESOURCE_GROUP,--dps-name,DPS_NAME,--enrollment-id,DEVICE_NAME,--attestation-type,x509,--certificate-path,CERT_PATH,--provisioning-status,enabled,--device-id,DEVICE_NAME'
ATTESTATION_SCRIPT_CMD              =    'iot,central,device,attestation,create,--app-id,APP_NAME,--device-id,DEVICE_NAME,--content'
ATTESTAION_CONTENT_JSON             =    '{\"type\":\"x509\",\"x509\":{\"clientCertificates\":{\"primary\":{\"certificate\":\"CERT\"},\"Secondary\":{\"certificate\":\"CERT\"}}}}'
ID_SCOPE_CMD                        =    'iot,central,device,show-credentials,--app-id,APP_NAME,--device-id,DEVICE_NAME,--query,"idScope"'

class AzHelper:

    def __init__(self, logger=None):
        self.az_cli = get_default_cli()
        self.logger = logging.getLogger(logger)

        #Disable Azure CLi Logging
        az_command_data_logger = logging.getLogger("az_command_data_logger")
        az_command_data_logger.disabled = True

        self.login()

# Run path in command line and output it to output.txt if logging level is greater than debug
    def cmd(self, cmd: list):
        if logging.DEBUG >= self.logger.level:
            self.az_cli.invoke(cmd)
        else:
            log = open('output.txt', 'a')
            self.az_cli.invoke(cmd,out_file=log)
            log.close()

        return self.az_cli.result

 ###############################################################################   
    def cmdPrint(self, cmdList: list, successMsg, errMsg, sysExit = True):
        cliCmdResult = self.cmd(cmdList)

        if cliCmdResult.exit_code != 0:
            self.logger.error(errMsg)
            if(sysExit):
                sys.exit()

        return cliCmdResult

###############################################################################   
    def login(self):
        self.cmdPrint(LOGIN_CMD.split(','),
                'Login Successful', 
                'Login Error')

###############################################################################   
    def checkResourceGroup(self, credentials):
        #self.logger.info('Checking Resource Group...')

        checkResourceGroupResult = self.cmdPrint(RESOURCE_GROUP_CHECK_CMD.replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"]).split(','),
								                '',
								                '',
								                 sysExit = False )
        return checkResourceGroupResult.result
    
###############################################################################   
    def createResourceGroup(self, credentials):
        self.cmdPrint(RESOURCE_GROUP_CMD.replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                           .replace('LOCATION',credentials["Entered"]["Location"])
                                           .split(','),
                                           'Resource Group Created', 
                                           'Resource Group Error')

###############################################################################   
    def checkIotHub(self, credentials):
        checkIOtHubResult = self.cmdPrint(CHECK_IOT_HUB_CMD.replace('HUB_NAME',credentials["Entered"]["IotHub"])
                                                 .replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                                 .split(','),
                                                 'Iot Hub Exist',
                                                 'Iot Hub Missing',
                                                  sysExit = False)

        if(checkIOtHubResult.exit_code == 0) :
            for hubIndex in checkIOtHubResult.result:
                if (hubIndex['name'] == credentials["Entered"]["IotHub"]):
                    return True

        return False

###############################################################################   
    def createIotHub(self, credentials):
        self.cmdPrint(IOT_HUB_CMD.replace('HUB_NAME', credentials["Entered"]["IotHub"])
                                    .replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                    .split(','),
                                    'Iot Hub Created', 
                                    'Iot Hub Error')
  
###############################################################################   
    def checkDPS(self, credentials):
        checkDpsResult = self.cmdPrint(CHECK_DPS_CMD.replace('DPS_NAME',credentials["Entered"]["DPS"])
                                                       .replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                                       .split(','),
                                                       'DPS exist',
                                                       'DPS missing',
                                                        sysExit = False)

        if(checkDpsResult.exit_code == 0) :
            for dpsIndex in checkDpsResult.result:
                if(dpsIndex['name'] == credentials["Entered"]["DPS"]) :
                    credentials["Generated"]['IDScope'] = dpsIndex['properties']['idScope']
                    return True

        return False

###############################################################################   
    def createDPS(self, credentials):
        createDPSResult = self.cmdPrint(DPS_CMD.replace('DPS_NAME',credentials["Entered"]["DPS"])
                                .replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                .split(','),
                                'DPS Created', 
                                'DPS Error')

        if createDPSResult.exit_code == 0 :
            credentials["Generated"]['IDScope'] = createDPSResult.result['properties']['idScope']


# check if DPS and HUB are already linked
    def checkLinkedHubDPS(self, credentials):
        linkHubDPSResult = self.cmdPrint(LINK_HUB_DPS_CHECK_CMD.replace('DPS_NAME', credentials["Entered"]["DPS"])
                                         .replace('RESOURCE_GROUP', credentials["Entered"]["ResourceGroup"])
                                         .split(','),
                                         '',
                                         '')

        if (linkHubDPSResult.exit_code == 0):
            for dpsIndex in linkHubDPSResult.result:
                if (dpsIndex['name'] == HUB_HOST_NAME_CMD.replace('HUB_NAME', credentials["Entered"]["IotHub"])):
                    #self.logger.info('DPS and HUB are linked...')
                    return True
        return False

###############################################################################   
    def linkHubDPS(self, credentials):
        linkHubDPSResult = self.cmdPrint(LINK_HUB_DPS_GET_CNX_STRING_CMD.replace('HUB_NAME',credentials["Entered"]["IotHub"])
                                                 .split(','),
                'Linked DPS Created', 
                'Linked DPS Error')
        if linkHubDPSResult.exit_code == 0:
            connectionString = linkHubDPSResult.result['connectionString']
            self.cmdPrint(LINK_HUB_DPS_SCRIPT_CMD.replace('DPS_NAME', credentials["Entered"]["DPS"]).replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                                                                                .replace('CONNECTION_STRING',connectionString)
                                                                                                .split(','),
                                                        'Linked DPS Created',
                                                        'Linked DPS Error')

###############################################################################   
    def checkIndividualEnrollment(self, credentials):
        checkEnrollementResult = self.cmdPrint(INDIVIDUAL_ENROLLMENT_CHECK_CMD.replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                               .replace('DPS_NAME',credentials["Entered"]["DPS"])
                                               .split(','),
                                                'Individual Enrollment Created',
                                                'Individual Enrollment Error')
        if(checkEnrollementResult.exit_code == 0 ):
            for enrollementIndex in checkEnrollementResult.result :
                if enrollementIndex['registrationId'] == credentials["Generated"]["RegistrationID"] :
                    return True
        return False
    
###############################################################################   
    def createIndividualEnrollment(self, credentials):
        self.cmdPrint(INDIVIDUAL_ENROLLMENT_CMD.replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                                  .replace('DPS_NAME',credentials["Entered"]["DPS"])
                                                  .replace('DEVICE_NAME',credentials["Generated"]["RegistrationID"])
                                                  .replace('CERT_PATH',credentials["Constant"]["CertPath"])
                                                  .split(','),
                                                  'Individual Enrollment Created',
                                                  'Individual Enrollment Error')

###############################################################################   
    def checkCentralApplication(self, credentials):
        createCentralApplicationResult = self.cmdPrint(CENTRAL_APPLICATION_CHECK_CMD.replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])
                                                          .split(','),
              '',
              '')

        if (createCentralApplicationResult.exit_code == 0):
            for appIndex in createCentralApplicationResult.result:
                if (appIndex['name'] == credentials["Entered"]["AppName"]):
                    webbrowser.open(IOT_CENTRAL_APP_URL.replace('APP_NAME', credentials["Entered"]["AppName"]), new=2)
                    credentials["Generated"]['AppID'] = appIndex['applicationId']
                    return True

        return False
    
###############################################################################   
    def createCentralApplication(self, credentials):
        applicationCmdParmas =  CENTRAL_APPLICATION_CMD.replace('RESOURCE_GROUP',credentials["Entered"]["ResourceGroup"])\
                                                          .replace('APP_NAME',credentials["Entered"]["AppName"])\
                                                          .replace('LOCATION',credentials["Entered"]["Location"])\
                                                          .replace('APP_TEMPLATE_ID',credentials["Constant"]["AppTemplateID"])\
                                                          .split(',')

        if(credentials["Constant"]["AppTemplateID"] != ""):
            applicationCmdParmas.append('--template')
            applicationCmdParmas.append(credentials["Constant"]["AppTemplateID"])

        createCentralApplicationResult = self.cmdPrint(applicationCmdParmas,
              'Central Application Created', 
              'Central Application Creation Error')

        if(createCentralApplicationResult.exit_code == 0):
            webbrowser.open(IOT_CENTRAL_APP_URL.replace('APP_NAME', credentials["Entered"]["AppName"]), new=2)
            credentials["Generated"]["AppID"] = createCentralApplicationResult.result
        else :
            self.logger.error('Error Creating Iot Central Application...')
            sys.exit()

###############################################################################   
    def createCentralDevice(self, credentials):
        deviceCmdParmas =  DEVICE_CMD.replace('APP_NAME',credentials["Entered"]["AppName"])\
                                        .replace('DEVICE_NAME',credentials["Generated"]["RegistrationID"])\
                                        .split(',')
        if(credentials["Constant"]["DevTemplateID"] != ""):
            deviceCmdParmas.append('--template')
            deviceCmdParmas.append(credentials["Constant"]["DevTemplateID"])

        self.cmdPrint(deviceCmdParmas,
                  'Device Created', 
                  'Device ERROR')

###############################################################################   
    def createCentralEnrollment(self, credentials):
        centralEnrollmentParams = ATTESTATION_SCRIPT_CMD.replace('APP_NAME',credentials["Entered"]["AppName"])\
                                                    .replace('DEVICE_NAME',credentials["Generated"]["RegistrationID"])\
                                                    .split(',')

        centralEnrollmentParams.append(ATTESTAION_CONTENT_JSON.replace('CERT',credentials["Generated"]["Cert"]))

        self.cmdPrint(centralEnrollmentParams,
                  'Device Created', 
                  'Device ERROR')

###############################################################################   
    def getCentralIDScope(self, credentials):
        CentralIDScopeResult = self.cmdPrint(ID_SCOPE_CMD.replace('APP_NAME',credentials["Entered"]["AppName"])
                                                            .replace('DEVICE_NAME',credentials["Generated"]["RegistrationID"])
                                                            .split(','),
                                              'ID Scope Collected',
                                              'ID Scope ERROR')

        if (CentralIDScopeResult.exit_code == 0):
            credentials["Generated"]["IDScope"] = CentralIDScopeResult.result
        else :
            self.logger.error('Collecting Scope ID failed ...')
