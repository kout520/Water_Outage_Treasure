################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/adc_going.c \
../code/esp32_concert.c \
../code/fun.c \
../code/key.c \
../code/lcd.c \
../code/pwm.c \
../code/timer.c \
../code/tjc_usart_hmi.c \
<<<<<<< HEAD
../code/water_quality.c \
../code/water_speed.c \
../code/zigbee.c 
=======
../code/water_speed.c 
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

C_DEPS += \
./code/adc_going.d \
./code/esp32_concert.d \
./code/fun.d \
./code/key.d \
./code/lcd.d \
./code/pwm.d \
./code/timer.d \
./code/tjc_usart_hmi.d \
<<<<<<< HEAD
./code/water_quality.d \
./code/water_speed.d \
./code/zigbee.d 
=======
./code/water_speed.d 
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

OBJS += \
./code/adc_going.o \
./code/esp32_concert.o \
./code/fun.o \
./code/key.o \
./code/lcd.o \
./code/pwm.o \
./code/timer.o \
./code/tjc_usart_hmi.o \
<<<<<<< HEAD
./code/water_quality.o \
./code/water_speed.o \
./code/zigbee.o 
=======
./code/water_speed.o 
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

DIR_OBJS += \
./code/*.o \

DIR_DEPS += \
./code/*.d \

DIR_EXPANDS += \
./code/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
code/%.o: ../code/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/Debug" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/Core" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/User" -I"c:/Users/kout/Desktop/Qiancomputer/CH32V307VCT/Peripheral/inc" -I"C:\Users\kout\Desktop\Qiancomputer\CH32V307VCT\code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

