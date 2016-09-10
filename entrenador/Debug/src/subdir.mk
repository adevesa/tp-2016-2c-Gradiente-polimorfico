################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/entrenador_commons.c 

OBJS += \
./src/entrenador_commons.o 

C_DEPS += \
./src/entrenador_commons.d 


# Each subdirectory must supply rules for building sources it contributes
src/entrenador_commons.o: ../src/entrenador_commons.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -include"/home/utnso/workspace/entrenador/src/entrenador_commons.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/entrenador_commons.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


