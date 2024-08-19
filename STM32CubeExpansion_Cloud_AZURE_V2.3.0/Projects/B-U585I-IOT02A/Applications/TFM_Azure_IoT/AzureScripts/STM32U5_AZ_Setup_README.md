# STM32U5 Azure Setup

## 1- Introduction
Welcome to the STM32U5 Azure Setup.

To ensure that [X-CUBE-AZURE](https://www.st.com/en/embedded-software/x-cube-azure.html) runs smoothly, it is recommended that you follow all the  STM32U5 Azure Setup steps bellow.

This process may take between 15mn to 1 hours to complete depending on your system and Internet speed.

If you have any question or need support, please contact our support line. The details are provided at the end of this document.

## 2- Important
You need to have administration rights on your PC to be able to install drivers and software and to do the workshop.

## 3- System requirements
* Windows machine with admin rights
* USB port access
* RECOMMENDED: Fresh Windows install, or Windows running on a virtual machine

## 4- Hardware Requirements
* 1 x [B-U585I-IOT02A](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html) board
* 1 x male Type-A to male Type-B micro-USB cable (not included in the kit)
* Windows PC
* 2.4 GHz Wi-Fi router (No captive portal)

## 5- Minimum Recommended Hardware Requirements for PC
* Type A USB port
* 2+ GHz processor
* 4 GB of system memory (RAM)
* 10 GB of available disk space

Note: For machines with USB Type C, please have a Type A to Type C adapter

## 6- Software Requirements
You need a Windows PC with administration rights to be able to install drivers and software and to do the workshop. 

We recommend a fresh Windows install, if that is not possible, try to run Windows sandbox or a virtual machine. It will be hard for us to debug system issues remotely.

| Software                | Version   | Description                                                            |
| ----------------------- | --------- |----------------------------------------------------------------------- |
| Git                     |           | DevOps tool used for source code management                               |
| STM32CubeProgrammer     | 2.12.0    | All-in-one multi-OS software tool for programming STM32                                    |
| X-CUBE-AZURE            | 2.1.0     | Integrate modular Azure RTOS kernel and libraries with hardware enforced security to build more secure cloud connected applications |
| Python with Pip         | 3.11.1    | A general-purpose programming language                                 |
| Azure CLI               | 2.40.0    | A tool that pulls all the Azure services together in one central command line interface |
| Edge or Chrome          |           | Web browser                                                            |
| WebEx Application       |           | Video Conference tool used for specific questions on the day of the session |


## 7- Prepare your system

### 7.1- Configure **PowerShell**

* Open PowerShell terminal as Administrator

![PowerShell](../Resources/PowerShell.jpg)


* Set execution policy

Run the following command to allow script execution

```
Set-ExecutionPolicy -ExecutionPolicy Unrestricted  -Scope CurrentUser
```
![RemoteSigned](../Resources/RemoteSigned.jpg)


Enter **y** to accept the Execution Policy Change

### 7.2- Navigate to the workshop directory

Navigate to the AzureScripts directory. 
```
cd C:\<Your directory>\AzureScripts
```

![Navigate](../Resources/Navigate.jpg)


### 7.3- Execute the script

Type .\STM32U5_AZ_Setup.ps1 to run the prerequisite check script

```
.\STM32U5_AZ_Setup.ps1
```

![RunScript](../Resources/RunScript.jpg)

### 7.4S Script flow
STM32U5_AZURE_QuickSetup programmatically installs all the software requirements to ensure that your environment is prepared for the workshop.

The flow is detailed here:
![Flow](../Resources/flow.jpg)


## 8- Wi-Fi Setup
You need a 2.4 GHz Wi-Fi to run the workshop as the Wi-Fi module on the  [B-U585I-IOT02A](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html) board supports 2.4 GHz Wi-Fi only. If you are not sure about yout Wi-Fi settings, you can setup your PC as a 2.4 GHz hotspot and connect the board to it. To do that:
* On the search bar type hotspot ![search_hotspot](../Resources/search_hotspot.jpg)
* Enable the hotspot ![hotspot_enable](../Resources/hotspot_enable.jpg)
* Edit the hotspot properties ![hotspot_edit_properties](../Resources/hotspot_edit_properties.jpg)
* Set the hotspot SSID, Password and Select the 2.4 GHz option  and click save ![hotspot_network_info](../Resources/hotspot_network_info.jpg)
* Turn the power saving option off ![hotspot_power_saving](../Resources/hotspot_power_saving.jpg)


## 9- Support

Questions and support

-	If you have issues during the software download and install, please contact ST by entering an Online Support Request at: https://community.st.com/s/onlinesupport?o=ws&tabset-08cae=2 
to help resolve the issue.

