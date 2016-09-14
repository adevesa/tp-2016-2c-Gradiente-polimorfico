################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../basic-structs.c \
../comunication-point.c \
../comunication.c \
../entrenador-commons.c \
../time.c 

OBJS += \
./basic-structs.o \
./comunication-point.o \
./comunication.o \
./entrenador-commons.o \
./time.o 

C_DEPS += \
./basic-structs.d \
./comunication-point.d \
./comunication.d \
./entrenador-commons.d \
./time.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


