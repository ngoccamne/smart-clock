################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/TFT_ILI9341/ILI9341_GFX.c \
../Libs/TFT_ILI9341/ILI9341_STM32_Driver.c \
../Libs/TFT_ILI9341/calibrate.c \
../Libs/TFT_ILI9341/fonts.c \
../Libs/TFT_ILI9341/graphics.c \
../Libs/TFT_ILI9341/ili9341.c \
../Libs/TFT_ILI9341/ili9341_touch.c \
../Libs/TFT_ILI9341/touch.c 

OBJS += \
./Libs/TFT_ILI9341/ILI9341_GFX.o \
./Libs/TFT_ILI9341/ILI9341_STM32_Driver.o \
./Libs/TFT_ILI9341/calibrate.o \
./Libs/TFT_ILI9341/fonts.o \
./Libs/TFT_ILI9341/graphics.o \
./Libs/TFT_ILI9341/ili9341.o \
./Libs/TFT_ILI9341/ili9341_touch.o \
./Libs/TFT_ILI9341/touch.o 

C_DEPS += \
./Libs/TFT_ILI9341/ILI9341_GFX.d \
./Libs/TFT_ILI9341/ILI9341_STM32_Driver.d \
./Libs/TFT_ILI9341/calibrate.d \
./Libs/TFT_ILI9341/fonts.d \
./Libs/TFT_ILI9341/graphics.d \
./Libs/TFT_ILI9341/ili9341.d \
./Libs/TFT_ILI9341/ili9341_touch.d \
./Libs/TFT_ILI9341/touch.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TFT_ILI9341/%.o Libs/TFT_ILI9341/%.su Libs/TFT_ILI9341/%.cyclo: ../Libs/TFT_ILI9341/%.c Libs/TFT_ILI9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Libs -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/Libs" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/DELAY" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/DHT11" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/DS3231" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/PIR" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/TFT_ILI9341" -I"D:/Project/Smart_Clock/Src/Firmware/App" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-TFT_ILI9341

clean-Libs-2f-TFT_ILI9341:
	-$(RM) ./Libs/TFT_ILI9341/ILI9341_GFX.cyclo ./Libs/TFT_ILI9341/ILI9341_GFX.d ./Libs/TFT_ILI9341/ILI9341_GFX.o ./Libs/TFT_ILI9341/ILI9341_GFX.su ./Libs/TFT_ILI9341/ILI9341_STM32_Driver.cyclo ./Libs/TFT_ILI9341/ILI9341_STM32_Driver.d ./Libs/TFT_ILI9341/ILI9341_STM32_Driver.o ./Libs/TFT_ILI9341/ILI9341_STM32_Driver.su ./Libs/TFT_ILI9341/calibrate.cyclo ./Libs/TFT_ILI9341/calibrate.d ./Libs/TFT_ILI9341/calibrate.o ./Libs/TFT_ILI9341/calibrate.su ./Libs/TFT_ILI9341/fonts.cyclo ./Libs/TFT_ILI9341/fonts.d ./Libs/TFT_ILI9341/fonts.o ./Libs/TFT_ILI9341/fonts.su ./Libs/TFT_ILI9341/graphics.cyclo ./Libs/TFT_ILI9341/graphics.d ./Libs/TFT_ILI9341/graphics.o ./Libs/TFT_ILI9341/graphics.su ./Libs/TFT_ILI9341/ili9341.cyclo ./Libs/TFT_ILI9341/ili9341.d ./Libs/TFT_ILI9341/ili9341.o ./Libs/TFT_ILI9341/ili9341.su ./Libs/TFT_ILI9341/ili9341_touch.cyclo ./Libs/TFT_ILI9341/ili9341_touch.d ./Libs/TFT_ILI9341/ili9341_touch.o ./Libs/TFT_ILI9341/ili9341_touch.su ./Libs/TFT_ILI9341/touch.cyclo ./Libs/TFT_ILI9341/touch.d ./Libs/TFT_ILI9341/touch.o ./Libs/TFT_ILI9341/touch.su

.PHONY: clean-Libs-2f-TFT_ILI9341

