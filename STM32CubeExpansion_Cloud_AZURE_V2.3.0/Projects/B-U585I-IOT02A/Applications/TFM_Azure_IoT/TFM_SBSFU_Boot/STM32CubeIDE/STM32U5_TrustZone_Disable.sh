#!/bin/bash -

#******************************************************************************
#* @file    STM32U5_TrustZone_Disable.bat
#* @author  MCD Application Team
#* @brief   Disables TrustZone on STM32U5
#******************************************************************************
# * Copyright (c) 2021 STMicroelectronics.
#
# * All rights reserved.
#
# * This software is licensed under terms that can be found in the LICENSE file
#
# * in the root directory of this software component.
#
# * If no LICENSE file comes with this software, it is provided AS-IS.
#******************************************************************************

echo Disabling trustzone requires that a trustzone enabled project is running on the board.
read -p 'Proceed (Y/[N]) ?' answer

if [ "$answer" != "Y" ] && [ "$answer" != "y" ]; then 
echo Aborted
exit 1
fi

# Determine OS to find Cube Programmer Path
if [[ "$OSTYPE" == "darwin"* ]]; then
	stm32programmercli=`which STM32_Programmer_CLI 2>/dev/null`
	if [ $? -ne 0 ]; then 
		stm32programmercli="/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI"
	fi 
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
	stm32programmercli=`which STM32_Programmer_CLI 2>/dev/null`
	if [ $? -ne 0 ]; then 
	    stm32programmercli="/home/$USER/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"
	fi
elif [[ "$OSTYPE" == "msys"* ]]; then
	stm32programmercli=`which STM32_Programmer_CLI 2>/dev/null`
	if [ $? -ne 0 ]; then 
	    stm32programmercli="C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe"
	    if [[ ! -f "$stm32programmercli" ]]; then
		    stm32programmercli="C:/Program Files (x86)/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe"
	    fi
	fi
else
    exit 1
fi


echo Disabling trustzone
"$stm32programmercli" -c port=SWD mode=HotPlug -ob nSWBOOT0=0 nBOOT0=0
"$stm32programmercli" -c port=SWD mode=HotPlug -ob RDP=0xDC
"$stm32programmercli" -c port=SWD mode=HotPlug -ob RDP=0xAA TZEN=0
"$stm32programmercli" -c port=SWD mode=HotPlug -ob nSWBOOT0=1 nBOOT0=1
echo Done

