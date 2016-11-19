################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../so-commons/collections/dictionary.c \
../so-commons/collections/list.c \
../so-commons/collections/queue.c 

OBJS += \
./so-commons/collections/dictionary.o \
./so-commons/collections/list.o \
./so-commons/collections/queue.o 

C_DEPS += \
./so-commons/collections/dictionary.d \
./so-commons/collections/list.d \
./so-commons/collections/queue.d 


# Each subdirectory must supply rules for building sources it contributes
so-commons/collections/%.o: ../so-commons/collections/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


