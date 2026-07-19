################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include FreeRTOS/portable/MemMang/subdir.mk
-include FreeRTOS/portable/GCC/RISC-V/subdir.mk
-include FreeRTOS/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
lwIP_FreeRTOS.hex \

SECONDARY_LIST += \
lwIP_FreeRTOS.lst \

SECONDARY_SIZE += \
lwIP_FreeRTOS.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: lwIP_FreeRTOS.elf secondary-outputs

# Tool invocations
lwIP_FreeRTOS.elf: $(OBJS) $(USER_OBJS_ESCAPE)
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -g -T "c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"lwIP_FreeRTOS.map" --specs=nano.specs --specs=nosys.specs -o "lwIP_FreeRTOS.elf" $(OBJS) $(LIBS)
lwIP_FreeRTOS.hex: lwIP_FreeRTOS.elf
	@	riscv-none-embed-objcopy -O ihex "lwIP_FreeRTOS.elf" "lwIP_FreeRTOS.hex"
lwIP_FreeRTOS.lst: lwIP_FreeRTOS.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "lwIP_FreeRTOS.elf" > "lwIP_FreeRTOS.lst"
lwIP_FreeRTOS.siz: lwIP_FreeRTOS.elf
	riscv-none-embed-size --format=berkeley "lwIP_FreeRTOS.elf"

# Other Targets
clean:
	-$(RM) $(DIR_OBJS) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE) lwIP_FreeRTOS.elf
	-$(RM) $(DIR_EXPANDS) $(CALLGRAPH_DOT)
	-$(RM) $(DIR_DEPS)
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets