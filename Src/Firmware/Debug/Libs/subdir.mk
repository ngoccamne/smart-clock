################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/DHT.c \
../Libs/Delay_us.c \
../Libs/delay.c \
../Libs/ili9341_display.c \
../Libs/pir_am312.c 

OBJS += \
./Libs/DHT.o \
./Libs/Delay_us.o \
./Libs/delay.o \
./Libs/ili9341_display.o \
./Libs/pir_am312.o 

C_DEPS += \
./Libs/DHT.d \
./Libs/Delay_us.d \
./Libs/delay.d \
./Libs/ili9341_display.d \
./Libs/pir_am312.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/%.o Libs/%.su Libs/%.cyclo: ../Libs/%.c Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Libs -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/DS3231" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs

clean-Libs:
	-$(RM) ./Libs/DHT.cyclo ./Libs/DHT.d ./Libs/DHT.o ./Libs/DHT.su ./Libs/Delay_us.cyclo ./Libs/Delay_us.d ./Libs/Delay_us.o ./Libs/Delay_us.su ./Libs/delay.cyclo ./Libs/delay.d ./Libs/delay.o ./Libs/delay.su ./Libs/ili9341_display.cyclo ./Libs/ili9341_display.d ./Libs/ili9341_display.o ./Libs/ili9341_display.su ./Libs/pir_am312.cyclo ./Libs/pir_am312.d ./Libs/pir_am312.o ./Libs/pir_am312.su

.PHONY: clean-Libs

