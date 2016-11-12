################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/basic-structs.c \
../src/comunicacion.c \
../src/deadlock-resolucion.c \
../src/deadlock.c \
../src/map-commons.c \
../src/mapa.c \
../src/planificador-rr.c \
../src/planificador.c \
../src/planificadorSRDF.c 

OBJS += \
./src/basic-structs.o \
./src/comunicacion.o \
./src/deadlock-resolucion.o \
./src/deadlock.o \
./src/map-commons.o \
./src/mapa.o \
./src/planificador-rr.o \
./src/planificador.o \
./src/planificadorSRDF.o 

C_DEPS += \
./src/basic-structs.d \
./src/comunicacion.d \
./src/deadlock-resolucion.d \
./src/deadlock.d \
./src/map-commons.d \
./src/mapa.d \
./src/planificador-rr.d \
./src/planificador.d \
./src/planificadorSRDF.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


