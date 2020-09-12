################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32l552zetxq.s 

OBJS += \
./Core/Startup/startup_stm32l552zetxq.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -c -x assembler-with-cpp --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

