################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/low_level_device.c \
/home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/low_level_flash.c \
/home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/main.c \
/home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/secure_nsc.c \
/home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/startup_stm32u5xx.c \
/home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/tick.c 

OBJS += \
./Application/User/low_level_device.o \
./Application/User/low_level_flash.o \
./Application/User/main.o \
./Application/User/secure_nsc.o \
./Application/User/startup_stm32u5xx.o \
./Application/User/tick.o 

C_DEPS += \
./Application/User/low_level_device.d \
./Application/User/low_level_flash.d \
./Application/User/main.d \
./Application/User/secure_nsc.d \
./Application/User/startup_stm32u5xx.d \
./Application/User/tick.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/low_level_device.o: /home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/low_level_device.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../Secure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/low_level_flash.o: /home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/low_level_flash.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../Secure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/main.o: /home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../Secure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/secure_nsc.o: /home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/secure_nsc.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../Secure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/startup_stm32u5xx.o: /home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/startup_stm32u5xx.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../Secure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/tick.o: /home/erhangok/Downloads/en.x-cube-azure-v2-3-0/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Projects/B-U585I-IOT02A/Applications/TFM_Azure_IoT/TFM_Loader/Secure/Src/tick.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U585xx -DBL2 -DUSE_HAL_DRIVER -DLOCAL_LOADER_CONFIG -c -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../Linker -I../../../Secure/Inc -I../../../Secure_nsclib -I../../../../../../../../Middlewares/Third_Party/trustedfirmware/platform/ext/driver -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/low_level_device.cyclo ./Application/User/low_level_device.d ./Application/User/low_level_device.o ./Application/User/low_level_device.su ./Application/User/low_level_flash.cyclo ./Application/User/low_level_flash.d ./Application/User/low_level_flash.o ./Application/User/low_level_flash.su ./Application/User/main.cyclo ./Application/User/main.d ./Application/User/main.o ./Application/User/main.su ./Application/User/secure_nsc.cyclo ./Application/User/secure_nsc.d ./Application/User/secure_nsc.o ./Application/User/secure_nsc.su ./Application/User/startup_stm32u5xx.cyclo ./Application/User/startup_stm32u5xx.d ./Application/User/startup_stm32u5xx.o ./Application/User/startup_stm32u5xx.su ./Application/User/tick.cyclo ./Application/User/tick.d ./Application/User/tick.o ./Application/User/tick.su

.PHONY: clean-Application-2f-User

