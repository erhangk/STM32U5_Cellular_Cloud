@echo off
setlocal enabledelayedexpansion

echo TFM_UPDATE started


set _flash_all=1

:: Parse arguments
:loop
if "%~1" == "--app-s" (
      set _app_s=1
      set _flash_all=0
      shift 
      goto :loop
)
if "%~1" == "--data-s" (
      set _data_s=1
      set _flash_all=0
      shift 
      goto :loop
)
if "%~1" == "--app-ns" (
      set _app_ns=1
      set _flash_all=0
      shift 
	  goto :loop
)
if "%~1" == "--data-ns" (
      set _data_ns=1
      set _flash_all=0
      shift 
	  goto :loop
)
if "%~1" == "--loader" (
      set _loader=1
      set _flash_all=0
      shift
	  goto :loop
)
if "%~1" == "--sbsfu" (
      set _sbsfu=1
      set _flash_all=0
      shift 
	  goto :loop
)
if not "%~1" == "" if not "%~1"=="AUTO" (
	echo Invalid arguments
	::exit 1
	goto :eof
)

if "%_flash_all%" == "1" (
	set _app_ns=1
    set _app_s=1
	set _data_s=1
	set _data_ns=1
	set _loader=1
	set _sbsfu=1
)

set stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
set connect_no_reset=-c port=SWD mode=UR
set connect=-c port=SWD mode=UR speed=Fast --hardRst
set slot0=0xc038000
set slot1=0xc068000
set slot2=0xc10a000
set slot3=0xc13a000
set slot4=0xc036000
set slot5=0xc108000
set slot6=0xc1da000
set slot7=0xc1dc000
set boot=0xc014000
set loader=0xc1fa000

set app_image_number=2

if  "%app_image_number%" == "2" (
if "%_app_s%" == "1" (
echo Writing TFM_Appli Secure
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_s_app_init.bin %slot0% -v
IF %errorlevel% NEQ 0 goto :error
echo TFM_Appli Secure Written
)
if "%_app_ns%" == "1" (
echo Writing TFM_Appli NonSecure
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_ns_app_init.bin %slot1% -v
IF %errorlevel% NEQ 0 goto :error
echo TFM_Appli NonSecure Written
)
)

if  "%app_image_number%" == "1" (
echo Writing TFM_Appli
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_app_init.bin %slot0% -v
IF %errorlevel% NEQ 0 goto :error
echo "TFM_Appli Written"
)

if  "%_data_s%" == "1" (
echo Writing TFM_Appli Secure Data
IF not exist %~dp0..\..\TFM_Appli\binary\tfm_s_data_init.bin (
@echo [31mError: tfm_s_data_init.bin does not exist! Run dataimg.bat script to generate it[0m
goto :error
)
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_s_data_init.bin %slot4% -v
IF %errorlevel% NEQ 0 goto :error
echo TFM_Appli Secure Data Written
)

if  "%_data_ns%" == "1" (
echo Writing TFM_Appli NonSecure Data
IF not exist %~dp0..\..\TFM_Appli\binary\tfm_ns_data_init.bin (
@echo [31mError: tfm_ns_data_init.bin does not exist! Run dataimg.bat script to generate it[0m
goto :error
)
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Appli\binary\tfm_ns_data_init.bin %slot5% -v
IF %errorlevel% NEQ 0 goto :error
echo TFM_Appli NonSecure Data Written
)

:: write loader if config loader is active
if  "%_loader%" == "1" (
echo Writing TFM_Loader
%stm32programmercli% %connect% -d %~dp0..\..\TFM_Loader\binary\loader.bin %loader% -v
IF %errorlevel% NEQ 0 goto :error
echo TFM_Loader  Written
)

if "%_sbsfu%" == "1" (
echo Writing TFM_SBSFU_Boot
%stm32programmercli% %connect% -d %~dp0..\Binary\bootloader.bin %boot% -v
IF %errorlevel% NEQ 0 goto :error
echo TFM_SBSFU_Boot Written
)
echo TFM_UPDATE script done

IF [%1] NEQ [AUTO] (
  pause
)
exit /B 0

:error
echo TFM_UPDATE script failed
IF [%1] NEQ [AUTO] (
  pause
)
exit /B 1

