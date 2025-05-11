################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/ds3231_i2c.c 

OBJS += \
./Libs/ds3231_i2c.o 

C_DEPS += \
./Libs/ds3231_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/%.o Libs/%.su Libs/%.cyclo: ../Libs/%.c Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../fonts -I"C:/Users/NGOC CAM/STM32CubeIDE/workspace_1.16.1/LCD_TFT_ILI9341/fonts" -I"C:/Users/NGOC CAM/STM32CubeIDE/workspace_1.16.1/LCD_TFT_ILI9341/Libs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs

clean-Libs:
	-$(RM) ./Libs/ds3231_i2c.cyclo ./Libs/ds3231_i2c.d ./Libs/ds3231_i2c.o ./Libs/ds3231_i2c.su

.PHONY: clean-Libs

