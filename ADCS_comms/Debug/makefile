################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include drivers/Src/subdir.mk
-include Startup/subdir.mk
-include Src/subdir.mk
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
ADCS_comms.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ADCS_comms.list \

OBJCOPY_BIN += \
ADCS_comms.bin \


# All Target
all: ADCS_comms.elf secondary-outputs

# Tool invocations
ADCS_comms.elf: $(OBJS) $(USER_OBJS) /home/adam/Documents/stm32_capstone/ADCS_comms/STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "ADCS_comms.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/adam/Documents/stm32_capstone/ADCS_comms/STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ADCS_comms.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ADCS_comms.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ADCS_comms.list"
	@echo 'Finished building: $@'
	@echo ' '

ADCS_comms.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ADCS_comms.bin"
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
