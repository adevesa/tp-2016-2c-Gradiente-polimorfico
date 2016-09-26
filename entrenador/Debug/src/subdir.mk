################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/basic-structs.c \
../src/comunication.c \
../src/entrenador-commons.c \
../src/entrenador.c \
../src/time.c 

OBJS += \
./src/basic-structs.o \
./src/comunication.o \
./src/entrenador-commons.o \
./src/entrenador.o \
./src/time.o 

C_DEPS += \
./src/basic-structs.d \
./src/comunication.d \
./src/entrenador-commons.d \
./src/entrenador.d \
./src/time.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


