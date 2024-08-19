:: arg1 is the build directory
@echo OFF
set "projectdir=%~dp0"
set "operation=%~1"
set "userAppBinary=%projectdir%\..\Binary"
::dummy nsc is generated only in config without secure
set "secure_nsc=%projectdir%\..\Secure_nsclib\secure_nsclib.o"
set "maxbytesize=440"
set "loader_s=%projectdir%\Secure\B-U585I-IOT02A\Exe\Project.bin"
set "loader_ns=%projectdir%\NonSecure\B-U585I-IOT02A\Exe\Project.bin"
set "binarydir=%projectdir%\..\Binary"
set "loader=%binarydir%\loader.bin"
set loader_ns_size=0x6000
set loader_s_size=0x4000
pushd %projectdir%\..\..\..\..\..\..\Middlewares\Third_Party\mcuboot
set mcuboot_dir=%cd%
popd
::line for window executable
set "imgtool=%mcuboot_dir%\scripts\dist\imgtool\imgtool.exe"
set "python="
if exist %imgtool% (
echo Postbuild with windows executable
goto postbuild
)
:py
::line for python
echo Postbuild with python script
set "imgtool=%mcuboot_dir%\scripts\imgtool\main.py"
set "python=python "
:postbuild
IF "%operation%" == "secure" (
goto secure_operation
)
::Make sure we have a Binary sub-folder in UserApp folder
if not exist "%binarydir%" (
mkdir "%binarydir%" 
)

::get size of secure_nsclib.o to determine MCUBOOT_PRIMARY_ONLY flag
FOR /F "usebackq" %%A IN ('%secure_nsc%') DO set filesize=%%~zA

if %filesize% LSS %maxbytesize% (
echo loader without secure part (MCUBOOT_PRIMARY_ONLY not defined) 
echo "loader without secure part (MCUBOOT_PRIMARY_ONLY not defined)" >> %projectdir%\output.txt
set "command=%python%%imgtool% ass  -i %loader_ns_size% %loader_ns% %loader% >> %projectdir%\output.txt 2>&1"
goto execute_command
)
:: loader with secure and non secure
echo loader with secure part (MCUBOOT_PRIMARY_ONLY defined)
echo "loader with secure part (MCUBOOT_PRIMARY_ONLY defined)" >> %projectdir%\output.txt
set "command=%python%%imgtool% ass -f %loader_s% -o %loader_s_size% -i %loader_ns_size% %loader_ns% %loader% >> %projectdir%\output.txt 2>&1"

:execute_command
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

if not exist "%loader%" (
exit /B 1
)
exit /B 0
:secure_operation
set "nsclib_destination=%projectdir%\..\Secure_nsclib\secure_nsclib.o"
set "nsclib_source=%projectdir%\Secure\B-U585I-IOT02A\Exe\Project_import_lib.o"
:: recopy non secure callable .o
set "command=copy %nsclib_source% %nsclib_destination%"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error
exit /B 0
:error
echo %command% : failed
echo "%command% : failed" >> %projectdir%\\output.txt
exit /B 1


