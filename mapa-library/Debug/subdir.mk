################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../basic-structs.c \
../comunicacion.c \
../comunication-point.c \
../map-commons.c \
../planificador-rr.c \
../planificador.c 

OBJS += \
./basic-structs.o \
./comunicacion.o \
./comunication-point.o \
./map-commons.o \
./planificador-rr.o \
./planificador.o 

C_DEPS += \
./basic-structs.d \
./comunicacion.d \
./comunication-point.d \
./map-commons.d \
./planificador-rr.d \
./planificador.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/nivel-gui" -I"/home/utnso/workspace/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


