################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/DS3231/ds3231_i2c.c 

OBJS += \
./Libs/DS3231/ds3231_i2c.o 

C_DEPS += \
./Libs/DS3231/ds3231_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/DS3231/%.o Libs/DS3231/%.su Libs/DS3231/%.cyclo: ../Libs/DS3231/%.c Libs/DS3231/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Libs -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Smart_Clock/Src/Firmware/Libs" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/DELAY" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/DHT11" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/DS3231" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/PIR" -I"D:/Project/Smart_Clock/Src/Firmware/Libs/TFT_ILI9341" -I"D:/Project/Smart_Clock/Src/Firmware/App" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-DS3231

clean-Libs-2f-DS3231:
	-$(RM) ./Libs/DS3231/ds3231_i2c.cyclo ./Libs/DS3231/ds3231_i2c.d ./Libs/DS3231/ds3231_i2c.o ./Libs/DS3231/ds3231_i2c.su

.PHONY: clean-Libs-2f-DS3231

