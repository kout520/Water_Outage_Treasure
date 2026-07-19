################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/MemMang/heap_4.c 

C_DEPS += \
./FreeRTOS/portable/MemMang/heap_4.d 

OBJS += \
./FreeRTOS/portable/MemMang/heap_4.o 

DIR_OBJS += \
./FreeRTOS/portable/MemMang/*.o \

DIR_DEPS += \
./FreeRTOS/portable/MemMang/*.d \

DIR_EXPANDS += \
./FreeRTOS/portable/MemMang/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/MemMang/%.o: ../FreeRTOS/portable/MemMang/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -g -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Debug" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Core" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/User" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Peripheral/inc" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/include" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/Common" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/MemMang" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

