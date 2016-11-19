################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../so-commons/bitarray.c \
../so-commons/config.c \
../so-commons/error.c \
../so-commons/log.c \
../so-commons/process.c \
../so-commons/string.c \
../so-commons/temporal.c \
../so-commons/txt.c 

OBJS += \
./so-commons/bitarray.o \
./so-commons/config.o \
./so-commons/error.o \
./so-commons/log.o \
./so-commons/process.o \
./so-commons/string.o \
./so-commons/temporal.o \
./so-commons/txt.o 

C_DEPS += \
./so-commons/bitarray.d \
./so-commons/config.d \
./so-commons/error.d \
./so-commons/log.d \
./so-commons/process.d \
./so-commons/string.d \
./so-commons/temporal.d \
./so-commons/txt.d 


# Each subdirectory must supply rules for building sources it contributes
so-commons/%.o: ../so-commons/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


