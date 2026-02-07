################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/MIN/min.c 

OBJS += \
./Libs/MIN/min.o 

C_DEPS += \
./Libs/MIN/min.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/MIN/%.o Libs/MIN/%.su Libs/MIN/%.cyclo: ../Libs/MIN/%.c Libs/MIN/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/App" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/DELAY" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/DHT11" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/DS3231" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/MIN" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/PIR" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/RINGBUFFER" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/App/UART" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-MIN

clean-Libs-2f-MIN:
	-$(RM) ./Libs/MIN/min.cyclo ./Libs/MIN/min.d ./Libs/MIN/min.o ./Libs/MIN/min.su

.PHONY: clean-Libs-2f-MIN

