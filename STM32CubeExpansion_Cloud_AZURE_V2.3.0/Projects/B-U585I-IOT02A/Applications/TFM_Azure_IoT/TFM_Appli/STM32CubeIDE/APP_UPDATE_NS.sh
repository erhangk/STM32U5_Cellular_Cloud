#!/bin/bash

# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
SCRIPTPATH=`dirname $SCRIPT`

$SCRIPTPATH/../../TFM_SBSFU_Boot/STM32CubeIDE/TFM_UPDATE.sh --app_ns


