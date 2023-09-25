################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/log/console.c \
../Core/Src/log/tstamp.c 

OBJS += \
./Core/Src/log/console.o \
./Core/Src/log/tstamp.o 

C_DEPS += \
./Core/Src/log/console.d \
./Core/Src/log/tstamp.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/log/%.o Core/Src/log/%.su Core/Src/log/%.cyclo: ../Core/Src/log/%.c Core/Src/log/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-log

clean-Core-2f-Src-2f-log:
	-$(RM) ./Core/Src/log/console.cyclo ./Core/Src/log/console.d ./Core/Src/log/console.o ./Core/Src/log/console.su ./Core/Src/log/tstamp.cyclo ./Core/Src/log/tstamp.d ./Core/Src/log/tstamp.o ./Core/Src/log/tstamp.su

.PHONY: clean-Core-2f-Src-2f-log

