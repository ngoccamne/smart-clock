################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DHT11/DHT.c 

OBJS += \
./DHT11/DHT.o 

C_DEPS += \
./DHT11/DHT.d 


# Each subdirectory must supply rules for building sources it contributes
DHT11/%.o DHT11/%.su DHT11/%.cyclo: ../DHT11/%.c DHT11/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Libs -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/DS3231" -I"D:/Project/Smart_Clock/Src/Firmware/DHT11" -I"D:/Project/Smart_Clock/Src/Firmware/TFT_ILI9341" -I"D:/Project/Smart_Clock/Src/Firmware/PIR" -I"D:/Project/Smart_Clock/Src/Firmware/DELAY" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-DHT11

clean-DHT11:
	-$(RM) ./DHT11/DHT.cyclo ./DHT11/DHT.d ./DHT11/DHT.o ./DHT11/DHT.su

.PHONY: clean-DHT11

