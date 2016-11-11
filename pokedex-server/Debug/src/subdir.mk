################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/borrados.c \
../src/comunication.c \
../src/file_manipuling.c \
../src/lectura_escritura.c \
../src/ls_and_atributes.c \
../src/operaciones.c \
../src/osada_generales.c \
../src/pokedex-server.c 

OBJS += \
./src/borrados.o \
./src/comunication.o \
./src/file_manipuling.o \
./src/lectura_escritura.o \
./src/ls_and_atributes.o \
./src/operaciones.o \
./src/osada_generales.o \
./src/pokedex-server.o 

C_DEPS += \
./src/borrados.d \
./src/comunication.d \
./src/file_manipuling.d \
./src/lectura_escritura.d \
./src/ls_and_atributes.d \
./src/operaciones.d \
./src/osada_generales.d \
./src/pokedex-server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


