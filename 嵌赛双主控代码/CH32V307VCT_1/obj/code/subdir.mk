################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/JLNU_OLED.c \
../code/MYI2C.c \
../code/adc_going.c \
../code/aht20.c \
../code/encoder.c \
../code/hc_sr04.c \
../code/humidity.c \
../code/pump_ctrl.c \
../code/temperature.c \
../code/timer.c \
../code/zigbee.c 

C_DEPS += \
./code/JLNU_OLED.d \
./code/MYI2C.d \
./code/adc_going.d \
./code/aht20.d \
./code/encoder.d \
./code/hc_sr04.d \
./code/humidity.d \
./code/pump_ctrl.d \
./code/temperature.d \
./code/timer.d \
./code/zigbee.d 

OBJS += \
./code/JLNU_OLED.o \
./code/MYI2C.o \
./code/adc_going.o \
./code/aht20.o \
./code/encoder.o \
./code/hc_sr04.o \
./code/humidity.o \
./code/pump_ctrl.o \
./code/temperature.o \
./code/timer.o \
./code/zigbee.o 

DIR_OBJS += \
./code/*.o \

DIR_DEPS += \
./code/*.d \

DIR_EXPANDS += \
./code/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
code/%.o: ../code/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT_1/Debug" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT_1/Core" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT_1/User" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT_1/Peripheral/inc" -I"C:\Users\kout\Desktop\Qiancomputer\CH32V307VCT_1\code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

