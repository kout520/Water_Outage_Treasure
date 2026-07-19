################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP-1.41/lwip-core/autoip.c \
../lwIP-1.41/lwip-core/def.c \
../lwIP-1.41/lwip-core/dhcp.c \
../lwIP-1.41/lwip-core/dns.c \
../lwIP-1.41/lwip-core/icmp.c \
../lwIP-1.41/lwip-core/igmp.c \
../lwIP-1.41/lwip-core/inet.c \
../lwIP-1.41/lwip-core/inet_chksum.c \
../lwIP-1.41/lwip-core/init.c \
../lwIP-1.41/lwip-core/ip.c \
../lwIP-1.41/lwip-core/ip_addr.c \
../lwIP-1.41/lwip-core/ip_frag.c \
../lwIP-1.41/lwip-core/mem.c \
../lwIP-1.41/lwip-core/memp.c \
../lwIP-1.41/lwip-core/netif.c \
../lwIP-1.41/lwip-core/pbuf.c \
../lwIP-1.41/lwip-core/raw.c \
../lwIP-1.41/lwip-core/stats.c \
../lwIP-1.41/lwip-core/sys.c \
../lwIP-1.41/lwip-core/tcp.c \
../lwIP-1.41/lwip-core/tcp_in.c \
../lwIP-1.41/lwip-core/tcp_out.c \
../lwIP-1.41/lwip-core/timers.c \
../lwIP-1.41/lwip-core/udp.c 

C_DEPS += \
./lwIP-1.41/lwip-core/autoip.d \
./lwIP-1.41/lwip-core/def.d \
./lwIP-1.41/lwip-core/dhcp.d \
./lwIP-1.41/lwip-core/dns.d \
./lwIP-1.41/lwip-core/icmp.d \
./lwIP-1.41/lwip-core/igmp.d \
./lwIP-1.41/lwip-core/inet.d \
./lwIP-1.41/lwip-core/inet_chksum.d \
./lwIP-1.41/lwip-core/init.d \
./lwIP-1.41/lwip-core/ip.d \
./lwIP-1.41/lwip-core/ip_addr.d \
./lwIP-1.41/lwip-core/ip_frag.d \
./lwIP-1.41/lwip-core/mem.d \
./lwIP-1.41/lwip-core/memp.d \
./lwIP-1.41/lwip-core/netif.d \
./lwIP-1.41/lwip-core/pbuf.d \
./lwIP-1.41/lwip-core/raw.d \
./lwIP-1.41/lwip-core/stats.d \
./lwIP-1.41/lwip-core/sys.d \
./lwIP-1.41/lwip-core/tcp.d \
./lwIP-1.41/lwip-core/tcp_in.d \
./lwIP-1.41/lwip-core/tcp_out.d \
./lwIP-1.41/lwip-core/timers.d \
./lwIP-1.41/lwip-core/udp.d 

OBJS += \
./lwIP-1.41/lwip-core/autoip.o \
./lwIP-1.41/lwip-core/def.o \
./lwIP-1.41/lwip-core/dhcp.o \
./lwIP-1.41/lwip-core/dns.o \
./lwIP-1.41/lwip-core/icmp.o \
./lwIP-1.41/lwip-core/igmp.o \
./lwIP-1.41/lwip-core/inet.o \
./lwIP-1.41/lwip-core/inet_chksum.o \
./lwIP-1.41/lwip-core/init.o \
./lwIP-1.41/lwip-core/ip.o \
./lwIP-1.41/lwip-core/ip_addr.o \
./lwIP-1.41/lwip-core/ip_frag.o \
./lwIP-1.41/lwip-core/mem.o \
./lwIP-1.41/lwip-core/memp.o \
./lwIP-1.41/lwip-core/netif.o \
./lwIP-1.41/lwip-core/pbuf.o \
./lwIP-1.41/lwip-core/raw.o \
./lwIP-1.41/lwip-core/stats.o \
./lwIP-1.41/lwip-core/sys.o \
./lwIP-1.41/lwip-core/tcp.o \
./lwIP-1.41/lwip-core/tcp_in.o \
./lwIP-1.41/lwip-core/tcp_out.o \
./lwIP-1.41/lwip-core/timers.o \
./lwIP-1.41/lwip-core/udp.o 


EXPANDS += \
./lwIP-1.41/lwip-core/autoip.c.234r.expand \
./lwIP-1.41/lwip-core/def.c.234r.expand \
./lwIP-1.41/lwip-core/dhcp.c.234r.expand \
./lwIP-1.41/lwip-core/dns.c.234r.expand \
./lwIP-1.41/lwip-core/icmp.c.234r.expand \
./lwIP-1.41/lwip-core/igmp.c.234r.expand \
./lwIP-1.41/lwip-core/inet.c.234r.expand \
./lwIP-1.41/lwip-core/inet_chksum.c.234r.expand \
./lwIP-1.41/lwip-core/init.c.234r.expand \
./lwIP-1.41/lwip-core/ip.c.234r.expand \
./lwIP-1.41/lwip-core/ip_addr.c.234r.expand \
./lwIP-1.41/lwip-core/ip_frag.c.234r.expand \
./lwIP-1.41/lwip-core/mem.c.234r.expand \
./lwIP-1.41/lwip-core/memp.c.234r.expand \
./lwIP-1.41/lwip-core/netif.c.234r.expand \
./lwIP-1.41/lwip-core/pbuf.c.234r.expand \
./lwIP-1.41/lwip-core/raw.c.234r.expand \
./lwIP-1.41/lwip-core/stats.c.234r.expand \
./lwIP-1.41/lwip-core/sys.c.234r.expand \
./lwIP-1.41/lwip-core/tcp.c.234r.expand \
./lwIP-1.41/lwip-core/tcp_in.c.234r.expand \
./lwIP-1.41/lwip-core/tcp_out.c.234r.expand \
./lwIP-1.41/lwip-core/timers.c.234r.expand \
./lwIP-1.41/lwip-core/udp.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
lwIP-1.41/lwip-core/%.o: ../lwIP-1.41/lwip-core/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -g -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Debug" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Core" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/User" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Peripheral/inc" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/include" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/Common" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/MemMang" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-arch" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-include" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-include/ipv4" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

