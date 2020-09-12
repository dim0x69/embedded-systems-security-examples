################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L5xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
P3_TZEN_NonSecure.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
P3_TZEN_NonSecure.list \

OBJCOPY_BIN += \
P3_TZEN_NonSecure.bin \


# All Target
all: P3_TZEN_NonSecure.elf secondary-outputs

# Tool invocations
P3_TZEN_NonSecure.elf: $(OBJS) $(USER_OBJS) C:\Users\dimi\STM32CubeIDE\workspace_1.3.0\P3_TZEN\NonSecure\STM32L552ZETXQ_FLASH.ld
	arm-none-eabi-gcc -o "P3_TZEN_NonSecure.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"C:\Users\dimi\STM32CubeIDE\workspace_1.3.0\P3_TZEN\NonSecure\STM32L552ZETXQ_FLASH.ld" --specs=nosys.specs -Wl,-Map="P3_TZEN_NonSecure.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

P3_TZEN_NonSecure.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "P3_TZEN_NonSecure.list"
	@echo 'Finished building: $@'
	@echo ' '

P3_TZEN_NonSecure.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "P3_TZEN_NonSecure.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
