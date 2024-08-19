#!/usr/bin/env python
# ******************************************************************************
# * @file    STU32U5_AZ_IotHub_QC.py
# * @author  MCD Application Team
# * @brief   Connect STM32U5 Discovery Kit to Azure Iot Hub
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

from scripts.az_helper import *
from scripts.stm32 import *
import json
import getopt
import sys
from pathlib import Path
from halo import Halo

REVISION = '1.1.0'

CONFIG_PATH =                     Path('./Config.json')
CERT_PATH =                       Path('./cert.pem')

BINARY_LIST =                    [Path('../TFM_Appli/Binary/tfm_ns_data_init.bin'),
                                  Path('../TFM_Appli/Binary/tfm_s_app_init.bin'),
                                  Path('../TFM_Appli/Binary/tfm_s_data_init.bin'),
                                  Path('../TFM_Appli/Binary/tfm_ns_app_init.bin'),
                                  Path('../TFM_Loader/Binary/loader.bin'),
                                  Path('../TFM_SBSFU_Boot/Binary/bootloader.bin')]

HELP = ['STM32U5_QuickConnnect.py options:', 
        '\n\t-d or --debug to view debug output',
        '\n\t-h or --help for help',
        '\n\t-i or --interactive to enter parameters interactivelty  at runtime', 
        '\n\t--Subscription=[subscription (y/n)]',
        '\n\t--AppName=[application name]',
        '\n\t--SSID=[wifi ssid]',
        '\n\t--Password=[wifi password]',
        '\n\t--ResourceGroup=[azure resource group (required with subscription)]',
        '\n\t--Location=[azure location (required with subscription)]']


useStm32ShellScripts = False
if sys.platform.startswith('win'):
  ## check if QuiCkconnect is running on PowerShell or Bash envirement #######
  import os, psutil, re
  pprocName = psutil.Process(os.getppid()).name()
  if(bool(re.fullmatch('pwsh|pwsh.exe|powershell.exe', pprocName)) == True):
    RESET_SCRIPT      = Path('../TFM_SBSFU_Boot/EWARM/STM32CubeProg_Reset.bat')
    REGRESSION_SCRIPT = Path('../TFM_SBSFU_Boot/EWARM/regression.bat')
    TFM_UPDATE_SCRIPT = Path('../TFM_SBSFU_Boot/EWARM/TFM_UPDATE.bat')
  else :
    useStm32ShellScripts = True
    RESET_SCRIPT      = Path('../TFM_SBSFU_Boot/STM32CubeIDE/STM32CubeProg_Reset.sh')
    REGRESSION_SCRIPT = Path('../TFM_SBSFU_Boot/STM32CubeIDE/regression.sh')
    TFM_UPDATE_SCRIPT = Path('../TFM_SBSFU_Boot/STM32CubeIDE/TFM_UPDATE.sh')
else :
  RESET_SCRIPT      = Path('../TFM_SBSFU_Boot/STM32CubeIDE/STM32CubeProg_Reset.sh')
  REGRESSION_SCRIPT = Path('../TFM_SBSFU_Boot/STM32CubeIDE/regression.sh')
  TFM_UPDATE_SCRIPT = Path('../TFM_SBSFU_Boot/STM32CubeIDE/TFM_UPDATE.sh')

# Check if binary files exist ##############################################################################################
def binariesExist(BINARY_LIST):
  logger = logging.getLogger(__name__)
  if os.path.exists("output.txt"):
    os.remove("output.txt")
  retVal = True
  for binary in BINARY_LIST:
    if not os.path.exists(binary):
      logger.error('ERR: Binary ' + str(binary) + ' does not exist')
      retVal = False
  return retVal

# Load credentials from the CONFIG_PATH into credentials dictionary ########################################################
def loadCredentials(CONFIG_PATH):
  configFile = open(CONFIG_PATH)
  credentials = json.load(configFile)
  configFile.close()
  return credentials

# Prompts user to enter credentials ########################################################################################
def getCredentials(credentials):
  logger = logging.getLogger(__name__)
  logger.info("Collecting configuration credentials. Press Enter to leave parameter unmodified:")

  credList = ["AppName", "SSID", "Password", "ResourceGroup", "Location"]

  # Request user to enter any empty fields

  for cred in credList:
    for retry in range(3):
      value = input(cred + "[" + credentials['Entered'][cred] + "]: ")
      if value.strip() != "":
        if cred == "AppName"  and ((all(char.islower() or char.isdecimal() or (char == '-') for char in value) == False) ):
          logger.warning("Avoid special characters, use lower case letters (a-z), numbers (0-9), and dashes (-)")
          continue
        credentials['Entered'][cred] = value.strip()
        break
      else :
        if credentials['Entered'][cred] == "" :
          logger.warning(cred + " Can't be empty ! ")
          if (retry == 2):
            logger.error("Error invalid " + cred)
            sys.exit()
        else:
          break
  
  return credentials

# Update Credentials File ##################################################################################################
def writeCredentialsToFile(credentials, CONFIG_PATH):
  logger = logging.getLogger(__name__)
  logger.debug("\nUpdating " + str(CONFIG_PATH) + "...\n")
  with open(CONFIG_PATH, 'w') as outfile:
    json.dump(credentials, outfile, indent=4)

# Write a credential to persistent storage #################################################################################
def writeCred(value, bit, u5:STM32):
  u5.send_cmd_read_response(bit)
  u5.send_cmd_read_response(value + '\r\n')

# Write Wi-Fi credentials to persistent storage ############################################################################
def writeCredentials(credentials, u5:STM32):
  ssid = credentials["Entered"]["SSID"]
  pswd = credentials["Entered"]["Password"]
  endpoint = credentials["Constant"]["Endpoint"]

  u5.reset()

  u5.waitPrompt('Do you want to change settings(Y/[N])?')
  u5.send_cmd_read_response('Y')

  writeCred(ssid, '2', u5)
  # For unsecured WiFi pswd might be empty so don't attempt to send it
  # or the device will get configured incorrectly with endpoint as the password
  if pswd:
    mask = ''
    for i in pswd: mask += '*'
    writeCred(pswd, '3', u5)

  writeCred(endpoint, '0', u5)

  u5.send_cmd_read_response('4')
  # Reset Device
  u5.send_cmd_read_response('8')

# Write IdScope to persistent storage ######################################################################################
def writeIdScope(credentials, u5:STM32):
  idscope = credentials["Generated"]["IDScope"]
  
  # Reset Device
  u5.reset()
  
  u5.waitPrompt('Do you want to change settings(Y/[N])?')
  u5.send_cmd_read_response('Y')

  writeCred(idscope, '1', u5)

  u5.send_cmd_read_response('4')
  # Reset Device
  u5.send_cmd_read_response('8')

# Read certificate from serial port ########################################################################################
def getCert(u5:STM32, CERT_PATH):
  logger = logging.getLogger(__name__)
  PEM_BEGIN_CRT = "-----BEGIN CERTIFICATE-----"
  PEM_END_CRT   = "-----END CERTIFICATE-----"
  cert_start_received = False
  cert_end_received = False
  cert = open(CERT_PATH,'w')
  formattedCert = ''

  # Find the Thing certificate and save it to cert.pem 
  ts = time.time()

  while cert_end_received == False:
    line = u5.read_line()
    if line.strip("\r\n\t") != '':
        logger.debug(line.strip("\r\n\t"))

    if PEM_BEGIN_CRT in line:
      cert_start_received = True

    if PEM_END_CRT in line:
      cert_end_received = True

    if cert_start_received == True:
      #cert.write(line.strip("\r\n\t") + '\n')
      line = line.strip("\r\n\t")
      cert.write(line + '\n')
      if PEM_BEGIN_CRT not in line and PEM_END_CRT not in line:
        formattedCert += line

    te = time.time()
    tt = te - ts;
    if( tt > 60):
      raise Exception ("Wi-Fi Connect timeout")

  cert.close()
  return formattedCert

# Read Common Name from serial port ########################################################################################
def getCN(u5:STM32):
  logger = logging.getLogger(__name__)
  THING_NAME_IS_STRING = "Registration ID:"
  thin_name_found = False
  # Find the Thing name and append it to config.txt
  while thin_name_found == False:
    line = u5.read_line()
    
    if THING_NAME_IS_STRING in line:
      thin_name_found = True

      try:
        index = line.index(THING_NAME_IS_STRING)
        idx1 = line.find(':')
        idx2 = len(line)
        cn = line[idx1+10:idx2-13]
      except ValueError:
        logger.error("ERR: Device Common Name Not Found")
        sys.exit()
  return cn

# Get the Cert and Common Name from serial output ##########################################################################
def getCertCN(credentials, u5:STM32):
  # Reset Board
  u5.reset()
  
  # Collect Cert and CN from serial
  formattedCert = getCert(u5, credentials["Constant"]["CertPath"])
  cn = getCN(u5)

  # Add Certificate and Registration to credentials
  credentials["Generated"]["Cert"] = formattedCert
  credentials["Generated"]["RegistrationID"] = cn
  writeCredentialsToFile(credentials, CONFIG_PATH)

  return credentials

# Checks to see if all the required resources to create an individual enrollment are populated in credentials file ##########
def resourceCheck(credentials):
  logger = logging.getLogger(__name__)
  retVal = True

  if not credentials['Entered']['ResourceGroup']:
    logger.error("ResourceGroup not found in " + str(CONFIG_PATH))
    retVal = False

  if 'AppID' not in credentials['Generated'] or not credentials['Generated']['AppID']:
    logger.error("AppID not found in " + str(CONFIG_PATH))
    retVal = False
  
  if not credentials['Entered']['SSID']:
    logger.error("SSID not found in " + str(CONFIG_PATH))
    retVal = False

  return retVal

# Prompts the user for (y/n) response based on msg input ###################################################################
def promptYN(msg):
  answer = ''
  while answer != 'y' and answer != 'n':
    value = input(msg)
    if(value != ''):
      answer = value.lower().strip()[0]
  return answer

# MAIN #####################################################################################################################
def main(argv):
  # Setting script config levels
  _interactive_level = False

  debug = False

  logging.basicConfig(format='%(levelname)s:\t%(message)s')
  logger = logging.getLogger(__name__)
  logger.setLevel(logging.INFO)

  # Load credentials dictionary from config path
  credentials = loadCredentials(CONFIG_PATH)

  spinner = Halo(text='Running ..', spinner='dots')

  # Collect Parameters from command line
  try:
    opts, args = getopt.getopt(argv,"hdiv", ["help", "debug","interactive","version", "AppName=", "DPS=", "IotHub=", "SSID=", "Password=", "ResourceGroup=", "Location=", "IDScope="])
  except getopt.GetoptError:
    print(*HELP)
    sys.exit()

  # Parsing command line arguments 
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      print(*HELP)
      sys.exit()
      
    elif opt in ("-d", "--debug"):
      logger.setLevel(logging.DEBUG)
      debug = True
      
    elif opt in ("-i", "--interactive"):
      _interactive_level = True
    
    elif opt in ("-v", "--version"):
      print(REVISION)
      sys.exit()
    
    elif opt in ("--AppName"):
      credentials['Entered']['AppName'] = arg

    elif opt in ("--DPS"):
      credentials['Entered']['DPS'] = arg

    elif opt in ("--IotHub"):
      credentials['Entered']['IotHub'] = arg

    elif opt in ("--SSID"):
      credentials['Entered']['SSID'] = arg

    elif opt in ("--Password"):
      credentials['Entered']['Password'] = arg

    elif opt in ("--ResourceGroup"):
      credentials['Entered']['ResourceGroup'] = arg

    elif opt in ("--Location"):
      credentials['Entered']['Location'] = arg

    elif opt in ("--IDScope"):
      credentials['Generated']['IDScope'] = arg

  # Ensure that binaries exist or exit execution
  if not binariesExist(BINARY_LIST):
    sys.exit()

  # Accept Parameters from user if interactive level is set
  if _interactive_level:
    credentials = getCredentials(credentials)

  writeCredentialsToFile(credentials, CONFIG_PATH)

  try:
    u5 = STM32(RESET_SCRIPT, REGRESSION_SCRIPT, TFM_UPDATE_SCRIPT, isShellScripts = useStm32ShellScripts)
  except Exception as e: 
    logger.error(e)
    logger.error("Please meks sure your board is plugged in and the comport is not used by another application/script.")
    exit()

  az = AzHelper(__name__)

  createCloud = True

  if 'AppID' in credentials['Generated'] and credentials['Generated']['AppID']:
    logger.warning("Application Found found in " + str(CONFIG_PATH) + ": " + credentials['Generated']['AppID'])
    if promptYN("Would you like to use a pre-configured Iot Central Application (Y/N)?") == 'y':
      createCloud = False

  if createCloud:
    logger.info('Checking Resource Group')
    if (az.checkResourceGroup(credentials) == False) :
      logger.info('Creating Resource Group')
      az.createResourceGroup(credentials)
    else:
      logger.info('ResourceGroup ' + credentials["Entered"]["ResourceGroup"] + ' already exist')
      

    logger.info('Checking Central Application')
    if (az.checkCentralApplication(credentials) == False):
      logger.info('Creating Central Application')
      az.createCentralApplication(credentials)
    else:
      logger.info('Central Application ' + credentials["Entered"]["AppName"] + ' already exist')

    writeCredentialsToFile(credentials, CONFIG_PATH)

  if not resourceCheck(credentials):
    logger.error("Miising information. Unable to setup device")
    sys.exit()

  logger.info("Setting Option Bytes")
  try:
    spinner.start()
    u5.regression()
  except Exception as e: 
    logger.error(e)
    exit()
  finally:  
    spinner.stop()

  logger.info("Flashing Firmware")
  try:
    spinner.start()
    u5.tfmUpdate()
  except Exception as e: 
    logger.error(e)
    exit()
  finally:  
    spinner.stop()

  logger.info("Writing Connection Parameters to Secure Storage")
  spinner.start("Waiting for the board to reboot")
  try:
    writeCredentials(credentials, u5)
  except Exception as e: 
    spinner.stop()
    logger.error(e)
    logger.info("Please run the STM32U5_TrustZone_Disable script, then power cycle the board")
    exit()
  finally:  
    spinner.stop()

  logger.info("Collecting X-509 Certificate and CN from STSAFE")
  spinner.start("Waiting for the board to connect to Wi-Fi")
  try:
    credentials = getCertCN(credentials, u5)
  except Exception as e: 
    spinner.stop()
    logger.error(e)
    logger.info("Please check if your 2.4 GHz Wi-Fi is ON, SSID and password ")
    exit()
  finally:  
    spinner.stop()

  logger.info('Creating Device')
  spinner.start("Running...")
  az.createCentralDevice(credentials)
  spinner.stop()

  logger.info('Creating Device X-509 Enrollment')
  spinner.start("Running...")
  az.createCentralEnrollment(credentials)
  spinner.stop()

  logger.info('Collecting Scope ID')
  spinner.start("Running...")
  az.getCentralIDScope(credentials)
  spinner.stop()

  logger.info("Writing ID Scope to Secure Storage")
  spinner.start("Waiting for the board to reboot")
  writeIdScope(credentials, u5)
  spinner.stop()

  writeCredentialsToFile(credentials, CONFIG_PATH)
  
  logger.info("Rebooting...")
  u5.reset()

  if debug == True:
    u5.serial_read()
  else:  
    spinner.start("Waiting for the board to connect to Azure IoT Central")
    try:
      u5.waitPrompt('Connected to IoTHub.')
    except:
      logger.error("Failed to connect to IoT central.")
      exit()
    
    spinner.stop()
    logger.info("Connected to Azure IoT Central")

if __name__ == "__main__":
  main(sys.argv[1:])
