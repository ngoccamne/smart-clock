################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/PIR/pir_am312.c 

OBJS += \
./Libs/PIR/pir_am312.o 

C_DEPS += \
./Libs/PIR/pir_am312.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/PIR/%.o Libs/PIR/%.su Libs/PIR/%.cyclo: ../Libs/PIR/%.c Libs/PIR/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/App" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/DELAY" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/DHT11" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/DS3231" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/MIN" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/PIR" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/Libs/RINGBUFFER" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_v1.0_STM32/App/UART" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-PIR

clean-Libs-2f-PIR:
	-$(RM) ./Libs/PIR/pir_am312.cyclo ./Libs/PIR/pir_am312.d ./Libs/PIR/pir_am312.o ./Libs/PIR/pir_am312.su

.PHONY: clean-Libs-2f-PIR

