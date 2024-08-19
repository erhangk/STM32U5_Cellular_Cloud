################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.S \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.S \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.S \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.S \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.S \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.S \
/home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.S 

OBJS += \
./Middlewares/threadx/port_gcc/tx_thread_context_restore.o \
./Middlewares/threadx/port_gcc/tx_thread_context_save.o \
./Middlewares/threadx/port_gcc/tx_thread_interrupt_control.o \
./Middlewares/threadx/port_gcc/tx_thread_schedule.o \
./Middlewares/threadx/port_gcc/tx_thread_stack_build.o \
./Middlewares/threadx/port_gcc/tx_thread_system_return.o \
./Middlewares/threadx/port_gcc/tx_timer_interrupt.o 

S_UPPER_DEPS += \
./Middlewares/threadx/port_gcc/tx_thread_context_restore.d \
./Middlewares/threadx/port_gcc/tx_thread_context_save.d \
./Middlewares/threadx/port_gcc/tx_thread_interrupt_control.d \
./Middlewares/threadx/port_gcc/tx_thread_schedule.d \
./Middlewares/threadx/port_gcc/tx_thread_stack_build.d \
./Middlewares/threadx/port_gcc/tx_thread_system_return.d \
./Middlewares/threadx/port_gcc/tx_timer_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/threadx/port_gcc/tx_thread_context_restore.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.S Middlewares/threadx/port_gcc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_DYNAMIC_MEMORY_ALLOCATION -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/threadx/port_gcc/tx_thread_context_save.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.S Middlewares/threadx/port_gcc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_DYNAMIC_MEMORY_ALLOCATION -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/threadx/port_gcc/tx_thread_interrupt_control.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.S Middlewares/threadx/port_gcc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_DYNAMIC_MEMORY_ALLOCATION -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/threadx/port_gcc/tx_thread_schedule.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.S Middlewares/threadx/port_gcc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_DYNAMIC_MEMORY_ALLOCATION -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/threadx/port_gcc/tx_thread_stack_build.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.S Middlewares/threadx/port_gcc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_DYNAMIC_MEMORY_ALLOCATION -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/threadx/port_gcc/tx_thread_system_return.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.S Middlewares/threadx/port_gcc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_DYNAMIC_MEMORY_ALLOCATION -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/threadx/port_gcc/tx_timer_interrupt.o: /home/erhangok/Documents/GitHub/STM32U5_Cellular_Cloud/STM32CubeExpansion_Cloud_AZURE_V2.3.0/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.S Middlewares/threadx/port_gcc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_DYNAMIC_MEMORY_ALLOCATION -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-threadx-2f-port_gcc

clean-Middlewares-2f-threadx-2f-port_gcc:
	-$(RM) ./Middlewares/threadx/port_gcc/tx_thread_context_restore.d ./Middlewares/threadx/port_gcc/tx_thread_context_restore.o ./Middlewares/threadx/port_gcc/tx_thread_context_save.d ./Middlewares/threadx/port_gcc/tx_thread_context_save.o ./Middlewares/threadx/port_gcc/tx_thread_interrupt_control.d ./Middlewares/threadx/port_gcc/tx_thread_interrupt_control.o ./Middlewares/threadx/port_gcc/tx_thread_schedule.d ./Middlewares/threadx/port_gcc/tx_thread_schedule.o ./Middlewares/threadx/port_gcc/tx_thread_stack_build.d ./Middlewares/threadx/port_gcc/tx_thread_stack_build.o ./Middlewares/threadx/port_gcc/tx_thread_system_return.d ./Middlewares/threadx/port_gcc/tx_thread_system_return.o ./Middlewares/threadx/port_gcc/tx_timer_interrupt.d ./Middlewares/threadx/port_gcc/tx_timer_interrupt.o

.PHONY: clean-Middlewares-2f-threadx-2f-port_gcc

