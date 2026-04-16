################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/core_riscv.c 

C_DEPS += \
./Core/core_riscv.d 

OBJS += \
./Core/core_riscv.o 

DIR_OBJS += \
./Core/*.o \

DIR_DEPS += \
./Core/*.d \

DIR_EXPANDS += \
./Core/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/Debug" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/Core" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/User" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/Peripheral/inc" -I"C:\Users\kout\Desktop\Qiancomputer\CH32V307VCT\code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

