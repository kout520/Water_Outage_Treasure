################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP-1.41/lwip-api/api_lib.c \
../lwIP-1.41/lwip-api/api_msg.c \
../lwIP-1.41/lwip-api/err.c \
../lwIP-1.41/lwip-api/netbuf.c \
../lwIP-1.41/lwip-api/netdb.c \
../lwIP-1.41/lwip-api/netifapi.c \
../lwIP-1.41/lwip-api/sockets.c \
../lwIP-1.41/lwip-api/tcpip.c 

C_DEPS += \
./lwIP-1.41/lwip-api/api_lib.d \
./lwIP-1.41/lwip-api/api_msg.d \
./lwIP-1.41/lwip-api/err.d \
./lwIP-1.41/lwip-api/netbuf.d \
./lwIP-1.41/lwip-api/netdb.d \
./lwIP-1.41/lwip-api/netifapi.d \
./lwIP-1.41/lwip-api/sockets.d \
./lwIP-1.41/lwip-api/tcpip.d 

OBJS += \
./lwIP-1.41/lwip-api/api_lib.o \
./lwIP-1.41/lwip-api/api_msg.o \
./lwIP-1.41/lwip-api/err.o \
./lwIP-1.41/lwip-api/netbuf.o \
./lwIP-1.41/lwip-api/netdb.o \
./lwIP-1.41/lwip-api/netifapi.o \
./lwIP-1.41/lwip-api/sockets.o \
./lwIP-1.41/lwip-api/tcpip.o 


EXPANDS += \
./lwIP-1.41/lwip-api/api_lib.c.234r.expand \
./lwIP-1.41/lwip-api/api_msg.c.234r.expand \
./lwIP-1.41/lwip-api/err.c.234r.expand \
./lwIP-1.41/lwip-api/netbuf.c.234r.expand \
./lwIP-1.41/lwip-api/netdb.c.234r.expand \
./lwIP-1.41/lwip-api/netifapi.c.234r.expand \
./lwIP-1.41/lwip-api/sockets.c.234r.expand \
./lwIP-1.41/lwip-api/tcpip.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
lwIP-1.41/lwip-api/%.o: ../lwIP-1.41/lwip-api/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -g -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Debug" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Core" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/User" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Peripheral/inc" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/include" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/Common" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/MemMang" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-arch" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-include" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-include/ipv4" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

