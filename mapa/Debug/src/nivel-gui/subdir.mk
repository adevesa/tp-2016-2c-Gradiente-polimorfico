################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/nivel-gui/nivel.c \
../src/nivel-gui/tad_items.c 

OBJS += \
./src/nivel-gui/nivel.o \
./src/nivel-gui/tad_items.o 

C_DEPS += \
./src/nivel-gui/nivel.d \
./src/nivel-gui/tad_items.d 


# Each subdirectory must supply rules for building sources it contributes
src/nivel-gui/%.o: ../src/nivel-gui/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


