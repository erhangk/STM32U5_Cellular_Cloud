################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/com.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/common.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/fw_update_app.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/low_level_device.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/low_level_flash.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/main.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/startup_stm32u5xx.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/tick.c \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/ymodem.c 

OBJS += \
./Application/User/com.o \
./Application/User/common.o \
./Application/User/fw_update_app.o \
./Application/User/low_level_device.o \
./Application/User/low_level_flash.o \
./Application/User/main.o \
./Application/User/startup_stm32u5xx.o \
./Application/User/tick.o \
./Application/User/ymodem.o 

C_DEPS += \
./Application/User/com.d \
./Application/User/common.d \
./Application/User/fw_update_app.d \
./Application/User/low_level_device.d \
./Application/User/low_level_flash.d \
./Application/User/main.d \
./Application/User/startup_stm32u5xx.d \
./Application/User/tick.d \
./Application/User/ymodem.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/com.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/com.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/common.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/common.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/fw_update_app.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/fw_update_app.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/low_level_device.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/low_level_device.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/low_level_flash.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/low_level_flash.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/main.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/startup_stm32u5xx.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/startup_stm32u5xx.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/tick.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/tick.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/ymodem.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/NonSecure/Src/ymodem.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../NonSecure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/com.cyclo ./Application/User/com.d ./Application/User/com.o ./Application/User/com.su ./Application/User/common.cyclo ./Application/User/common.d ./Application/User/common.o ./Application/User/common.su ./Application/User/fw_update_app.cyclo ./Application/User/fw_update_app.d ./Application/User/fw_update_app.o ./Application/User/fw_update_app.su ./Application/User/low_level_device.cyclo ./Application/User/low_level_device.d ./Application/User/low_level_device.o ./Application/User/low_level_device.su ./Application/User/low_level_flash.cyclo ./Application/User/low_level_flash.d ./Application/User/low_level_flash.o ./Application/User/low_level_flash.su ./Application/User/main.cyclo ./Application/User/main.d ./Application/User/main.o ./Application/User/main.su ./Application/User/startup_stm32u5xx.cyclo ./Application/User/startup_stm32u5xx.d ./Application/User/startup_stm32u5xx.o ./Application/User/startup_stm32u5xx.su ./Application/User/tick.cyclo ./Application/User/tick.d ./Application/User/tick.o ./Application/User/tick.su ./Application/User/ymodem.cyclo ./Application/User/ymodem.d ./Application/User/ymodem.o ./Application/User/ymodem.su

.PHONY: clean-Application-2f-User

