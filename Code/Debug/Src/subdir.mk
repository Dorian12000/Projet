################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../dev_lidar/Src/drv_lidar.c \
../dev_lidar/Src/lidarTask.c 

OBJS += \
./Src/drv_lidar.o \
./Src/lidarTask.o 

C_DEPS += \
./Src/drv_lidar.d \
./Src/lidarTask.d 


# Each subdirectory must supply rules for building sources it contributes
Src/drv_lidar.o: C:/Users/doria/OneDrive/Documents/ENSEA/ESE/robot/Projet/Code/dev_lidar/Src/drv_lidar.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"C:/Users/doria/OneDrive/Documents/ENSEA/ESE/robot/Projet/Code/dev_lidar/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/lidarTask.o: C:/Users/doria/OneDrive/Documents/ENSEA/ESE/robot/Projet/Code/dev_lidar/Src/lidarTask.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"C:/Users/doria/OneDrive/Documents/ENSEA/ESE/robot/Projet/Code/dev_lidar/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/drv_lidar.cyclo ./Src/drv_lidar.d ./Src/drv_lidar.o ./Src/drv_lidar.su ./Src/lidarTask.cyclo ./Src/lidarTask.d ./Src/lidarTask.o ./Src/lidarTask.su

.PHONY: clean-Src

