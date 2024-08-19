@echo off
setlocal enabledelayedexpansion
set projectdir="%~dp0"
%projectdir%\..\..\TFM_SBSFU_Boot\EWARM\TFM_UPDATE.bat --app-ns %1
