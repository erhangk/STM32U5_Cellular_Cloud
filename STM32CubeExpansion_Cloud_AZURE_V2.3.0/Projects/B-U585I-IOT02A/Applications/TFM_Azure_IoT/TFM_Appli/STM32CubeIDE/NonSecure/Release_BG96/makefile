################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Release/subdir.mk
-include Middlewares/trustedfirmware/core/subdir.mk
-include Middlewares/threadx/port_gcc/subdir.mk
-include Middlewares/threadx/core/subdir.mk
-include Middlewares/netxduo/tls/subdir.mk
-include Middlewares/netxduo/drivers/Cellular/subdir.mk
-include Middlewares/netxduo/crypto/subdir.mk
-include Middlewares/netxduo/core/subdir.mk
-include Middlewares/netxduo/addons/web/subdir.mk
-include Middlewares/netxduo/addons/sntp/subdir.mk
-include Middlewares/netxduo/addons/mqtt/subdir.mk
-include Middlewares/netxduo/addons/dns/subdir.mk
-include Middlewares/netxduo/addons/dhcp/subdir.mk
-include Middlewares/netxduo/addons/cloud/subdir.mk
-include Middlewares/netxduo/addons/azure_iot/subdir.mk
-include Middlewares/cmsis_rtos_threadx/subdir.mk
-include Middlewares/Cellular/Misc/Cmd/subdir.mk
-include Middlewares/Cellular/Interface/Control/subdir.mk
-include Middlewares/Cellular/Interface/Com/subdir.mk
-include Middlewares/Cellular/Core/Trace/subdir.mk
-include Middlewares/Cellular/Core/Runtime_Library/subdir.mk
-include Middlewares/Cellular/Core/Rtosal/subdir.mk
-include Middlewares/Cellular/Core/Ipc/subdir.mk
-include Middlewares/Cellular/Core/Error/subdir.mk
-include Middlewares/Cellular/Core/Data_Cache/subdir.mk
-include Middlewares/Cellular/Core/Cellular_Service/subdir.mk
-include Middlewares/Cellular/Core/AT_Core/subdir.mk
-include Drivers/STM32U5xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/Components/cellular/BG96/subdir.mk
-include Drivers/BSP/Components/STDIO/subdir.mk
-include Drivers/BSP/Components/subdir.mk
-include Drivers/BSP/B-U585I-IOT02A/subdir.mk
-include Application/User/subdir.mk
-include Application/PSA_Crypto_Ciphersuites/subdir.mk
-include Application/NetXDuo/App/subdir.mk
-include Application/AZURE_RTOS/App/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := TFM_Appli_NonSecure
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
TFM_Appli_NonSecure.elf \

MAP_FILES += \
TFM_Appli_NonSecure.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
TFM_Appli_NonSecure.list \


# All Target
all:
	+@$(MAKE) --no-print-directory pre-build && $(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: TFM_Appli_NonSecure.elf secondary-outputs

# Tool invocations
TFM_Appli_NonSecure.elf TFM_Appli_NonSecure.map: $(OBJS) $(USER_OBJS) ./output.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "TFM_Appli_NonSecure.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"./output.ld" --specs=nosys.specs -Wl,-Map="TFM_Appli_NonSecure.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

TFM_Appli_NonSecure.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "TFM_Appli_NonSecure.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) TFM_Appli_NonSecure.elf TFM_Appli_NonSecure.list TFM_Appli_NonSecure.map default.size.stdout
	-@echo ' '

pre-build:
	arm-none-eabi-gcc -E -P -xc -DSTM32U585xx -DBL2 -DTFM_PSA_API  -I../../../../Linker -o./output.ld  ../stm32u5xx_ns.ld
	-@echo ' '

post-build:
	mkdir -p ../Release
	arm-none-eabi-objcopy -O binary "TFM_Appli_NonSecure.elf" "../Release/TFM_Appli_NonSecure.bin"
	cp "TFM_Appli_NonSecure.elf" "../Release/TFM_Appli_NonSecure.elf"
	"../../postbuild.sh" "."  "-" "nonsecure"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified pre-build post-build

-include ../makefile.targets
