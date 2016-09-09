################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../arboles-directorios.c \
../map-commons.c \
../pokenest-commons.c 

OBJS += \
./arboles-directorios.o \
./map-commons.o \
./pokenest-commons.o 

C_DEPS += \
./arboles-directorios.d \
./map-commons.d \
./pokenest-commons.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


