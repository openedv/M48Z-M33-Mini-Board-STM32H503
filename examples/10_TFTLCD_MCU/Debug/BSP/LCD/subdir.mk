################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/LCD/lcd.c 

OBJS += \
./BSP/LCD/lcd.o 

C_DEPS += \
./BSP/LCD/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/LCD/%.o BSP/LCD/%.su BSP/LCD/%.cyclo: ../BSP/LCD/%.c BSP/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H503xx -c -I../Core/Inc -I../../../libraries/Drivers/STM32H5xx_HAL_Driver/Inc -I../../../libraries/Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../../../libraries/Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../../../libraries/Drivers/CMSIS/Include -I../BSP/LED -I../BSP/LCD -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP-2f-LCD

clean-BSP-2f-LCD:
	-$(RM) ./BSP/LCD/lcd.cyclo ./BSP/LCD/lcd.d ./BSP/LCD/lcd.o ./BSP/LCD/lcd.su

.PHONY: clean-BSP-2f-LCD

