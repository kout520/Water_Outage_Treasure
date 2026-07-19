################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/Jubei_eth_driver.c 

C_DEPS += \
./BSP/Jubei_eth_driver.d 

OBJS += \
./BSP/Jubei_eth_driver.o 


EXPANDS += \
./BSP/Jubei_eth_driver.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
BSP/%.o: ../BSP/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -g -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Debug" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Core" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/User" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/BSP" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/SRC/Peripheral/inc" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/include" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/Common" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/FreeRTOS/portable/MemMang" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-arch" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-include" -I"d:/ch32/QS/CH32V307_lwIP-1.41_FreeRTOS-main/lwIP-1.41_FreeRTOS/FreeRTOS/lwIP-1.41/lwip-include/ipv4" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

