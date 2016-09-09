################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cunitdeclarations.c \
../src/mapa-test.c \
../src/mapa-testing.c 

OBJS += \
./src/cunitdeclarations.o \
./src/mapa-test.o \
./src/mapa-testing.o 

C_DEPS += \
./src/cunitdeclarations.d \
./src/mapa-test.d \
./src/mapa-testing.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/mapa-prueba" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


