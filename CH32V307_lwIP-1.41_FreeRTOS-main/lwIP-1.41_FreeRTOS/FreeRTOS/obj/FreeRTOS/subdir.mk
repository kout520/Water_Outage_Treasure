################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/croutine.c \
../FreeRTOS/event_groups.c \
../FreeRTOS/list.c \
../FreeRTOS/queue.c \
../FreeRTOS/stream_buffer.c \
../FreeRTOS/tasks.c \
../FreeRTOS/timers.c 

C_DEPS += \
./FreeRTOS/croutine.d \
./FreeRTOS/event_groups.d \
./FreeRTOS/list.d \
./FreeRTOS/queue.d \
./FreeRTOS/stream_buffer.d \
./FreeRTOS/tasks.d \
./FreeRTOS/timers.d 

OBJS += \
./FreeRTOS/croutine.o \
./FreeRTOS/event_groups.o \
./FreeRTOS/list.o \
./FreeRTOS/queue.o \
./FreeRTOS/stream_buffer.o \
./FreeRTOS/tasks.o \
./FreeRTOS/timers.o 

DIR_OBJS += \
./FreeRTOS/*.o \

DIR_DEPS += \
./FreeRTOS/*.d \

DIR_EXPANDS += \
./FreeRTOS/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/%.o: ../FreeRTOS/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -g -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Debug" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Core" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/User" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Peripheral/inc" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/include" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/Common" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V" -I"c:/Users/kout/Desktop/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/MemMang" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

