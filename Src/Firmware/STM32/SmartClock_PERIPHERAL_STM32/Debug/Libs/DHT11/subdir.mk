################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/DHT11/DHT.c 

OBJS += \
./Libs/DHT11/DHT.o 

C_DEPS += \
./Libs/DHT11/DHT.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/DHT11/%.o Libs/DHT11/%.su Libs/DHT11/%.cyclo: ../Libs/DHT11/%.c Libs/DHT11/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/Libs" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/Libs/DS3231" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/Libs/PIR" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/Libs/DHT11" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/DELAY" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/App" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/App/UART" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/Libs/MIN" -I"D:/Project/Smart_Clock/Src/Firmware/STM32/SmartClock_PERIPHERAL_STM32/Libs/RingBuffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-DHT11

clean-Libs-2f-DHT11:
	-$(RM) ./Libs/DHT11/DHT.cyclo ./Libs/DHT11/DHT.d ./Libs/DHT11/DHT.o ./Libs/DHT11/DHT.su

.PHONY: clean-Libs-2f-DHT11

