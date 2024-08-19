#!/bin/bash -

#******************************************************************************
#* @file    STM32CubeProg_Reset.bat
#* @author  MCD Application Team
#* @brief   Issue a reset using ST-Link and STM32CubeProg.
#******************************************************************************
#* @attention
#*
#* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
#*
#* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
#* You may not use this file except in compliance with the License.
#* You may obtain a copy of the License at:
#*
#*        http://www.st.com/software_license_agreement_liberty_v2
#*
#* Unless required by applicable law or agreed to in writing, software 
#* distributed under the License is distributed on an "AS IS" BASIS, 
#* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#* See the License for the specific language governing permissions and
#* limitations under the License.
#******************************************************************************


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


"$stm32programmercli" -c port=swd -hardrst > /dev/null
if [ $? -ne 0 ]; then
	echo Board reset...failed
	exit 1
fi
echo Board reset...OK

