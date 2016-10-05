################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/comunication-with-server.c \
../src/fuse-basic.c \
../src/podekex-cliente.c 

OBJS += \
./src/comunication-with-server.o \
./src/fuse-basic.o \
./src/podekex-cliente.o 

C_DEPS += \
./src/comunication-with-server.d \
./src/fuse-basic.d \
./src/podekex-cliente.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


