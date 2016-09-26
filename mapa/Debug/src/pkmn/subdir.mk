################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/pkmn/battle.c \
../src/pkmn/factory.c 

OBJS += \
./src/pkmn/battle.o \
./src/pkmn/factory.o 

C_DEPS += \
./src/pkmn/battle.d \
./src/pkmn/factory.d 


# Each subdirectory must supply rules for building sources it contributes
src/pkmn/%.o: ../src/pkmn/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


