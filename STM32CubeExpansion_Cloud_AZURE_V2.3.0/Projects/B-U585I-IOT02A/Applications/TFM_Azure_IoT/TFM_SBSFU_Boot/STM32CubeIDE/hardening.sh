#!/bin/bash -
sec1_end=51
sec2_start=127
sec2_end=0
wrp_start=10
wrp_end=20
hdp_end=19
wrp_bank2_start=125
wrp_bank2_end=127
nsbootadd=0x1802c0
echo "hardening script started"


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
	if [ "$1" != "AUTO" ]; then read -p "Unable to find STM32_Programmer_CLI, press a key" -n1 -s; fi
    exit 1
fi

bank2_secure="-ob SECWM2_PSTRT="$sec2_start" SECWM2_PEND="$sec2_end
connect="-c port=SWD mode=UR --hardRst"
connect_no_reset="-c port=SWD mode=HotPlug"
wrp_loader="WRP2A_PSTRT="$wrp_bank2_start" WRP2A_PEND="$wrp_bank2_end
wrp_sbsfu="WRP1A_PSTRT="$wrp_start" WRP1A_PEND="$wrp_end
nsboot_add_set="NSBOOTADD0="$nsbootadd" NSBOOTADD1="$nsbootadd
write_protect_secure="-ob "$wrp_sbsfu" "$wrp_loader" SECWM1_PEND="$sec1_end" HDP1_PEND="$hdp_end" HDP1EN=1 "$nsboot_add_set" BOOT_LOCK=1"
"$stm32programmercli" $connect $bank2_secure
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "hardening script failed, press a key" -n1 -s; fi
  exit 1
fi
"$stm32programmercli" $connect $write_protect_secure
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "hardening script failed, press a key" -n1 -s; fi
  exit 1
fi
if [ "$1" != "AUTO" ]; then read -p "hardening script Done, press a key" -n1 -s; fi
exit 0