################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/f303xx_spi_driver.c \
../drivers/Src/stm32f303xx_gpio_driver.c 

OBJS += \
./drivers/Src/f303xx_spi_driver.o \
./drivers/Src/stm32f303xx_gpio_driver.o 

C_DEPS += \
./drivers/Src/f303xx_spi_driver.d \
./drivers/Src/stm32f303xx_gpio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o drivers/Src/%.su drivers/Src/%.cyclo: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303VCTx -DSTM32 -DSTM32F3 -DSTM32F3DISCOVERY -c -I../Inc -I"C:/education/embedded/c/udemy/MCU1/f303xx_drivers/drivers/Inc" -I"C:/education/embedded/c/udemy/MCU1/f303xx_drivers/drivers" -I"C:/education/embedded/c/udemy/MCU1/f303xx_drivers/drivers/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/f303xx_spi_driver.cyclo ./drivers/Src/f303xx_spi_driver.d ./drivers/Src/f303xx_spi_driver.o ./drivers/Src/f303xx_spi_driver.su ./drivers/Src/stm32f303xx_gpio_driver.cyclo ./drivers/Src/stm32f303xx_gpio_driver.d ./drivers/Src/stm32f303xx_gpio_driver.o ./drivers/Src/stm32f303xx_gpio_driver.su

.PHONY: clean-drivers-2f-Src
