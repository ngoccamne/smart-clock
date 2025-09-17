################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TFT_ILI9341/ILI9341_GFX.c \
../TFT_ILI9341/ILI9341_STM32_Driver.c \
../TFT_ILI9341/fonts.c \
../TFT_ILI9341/ili9341_display.c 

OBJS += \
./TFT_ILI9341/ILI9341_GFX.o \
./TFT_ILI9341/ILI9341_STM32_Driver.o \
./TFT_ILI9341/fonts.o \
./TFT_ILI9341/ili9341_display.o 

C_DEPS += \
./TFT_ILI9341/ILI9341_GFX.d \
./TFT_ILI9341/ILI9341_STM32_Driver.d \
./TFT_ILI9341/fonts.d \
./TFT_ILI9341/ili9341_display.d 


# Each subdirectory must supply rules for building sources it contributes
TFT_ILI9341/%.o TFT_ILI9341/%.su TFT_ILI9341/%.cyclo: ../TFT_ILI9341/%.c TFT_ILI9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Libs -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/DS3231" -I"D:/Project/Smart_Clock/Src/Firmware/DHT11" -I"D:/Project/Smart_Clock/Src/Firmware/TFT_ILI9341" -I"D:/Project/Smart_Clock/Src/Firmware/PIR" -I"D:/Project/Smart_Clock/Src/Firmware/DELAY" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TFT_ILI9341

clean-TFT_ILI9341:
	-$(RM) ./TFT_ILI9341/ILI9341_GFX.cyclo ./TFT_ILI9341/ILI9341_GFX.d ./TFT_ILI9341/ILI9341_GFX.o ./TFT_ILI9341/ILI9341_GFX.su ./TFT_ILI9341/ILI9341_STM32_Driver.cyclo ./TFT_ILI9341/ILI9341_STM32_Driver.d ./TFT_ILI9341/ILI9341_STM32_Driver.o ./TFT_ILI9341/ILI9341_STM32_Driver.su ./TFT_ILI9341/fonts.cyclo ./TFT_ILI9341/fonts.d ./TFT_ILI9341/fonts.o ./TFT_ILI9341/fonts.su ./TFT_ILI9341/ili9341_display.cyclo ./TFT_ILI9341/ili9341_display.d ./TFT_ILI9341/ili9341_display.o ./TFT_ILI9341/ili9341_display.su

.PHONY: clean-TFT_ILI9341

