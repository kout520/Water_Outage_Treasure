################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Startup/startup_ch32v30x_D8C.S 

S_UPPER_DEPS += \
./Startup/startup_ch32v30x_D8C.d 

OBJS += \
./Startup/startup_ch32v30x_D8C.o 

DIR_OBJS += \
./Startup/*.o \

DIR_DEPS += \
./Startup/*.d \

DIR_EXPANDS += \
./Startup/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.S
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -g -x assembler-with-cpp -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/Startup" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/include" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/Common" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/MemMang" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

