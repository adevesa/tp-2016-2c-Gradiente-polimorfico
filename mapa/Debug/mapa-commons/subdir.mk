################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mapa-commons/comunicacion.c \
../mapa-commons/map-commons.c \
../mapa-commons/pokenest-commons.c 

OBJS += \
./mapa-commons/comunicacion.o \
./mapa-commons/map-commons.o \
./mapa-commons/pokenest-commons.o 

C_DEPS += \
./mapa-commons/comunicacion.d \
./mapa-commons/map-commons.d \
./mapa-commons/pokenest-commons.d 


# Each subdirectory must supply rules for building sources it contributes
mapa-commons/%.o: ../mapa-commons/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/cliente-servidor" -I"/home/utnso/workspace/directorios-commons" -I"/home/utnso/workspace/commons" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mapa-commons/map-commons.o: ../mapa-commons/map-commons.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/cliente-servidor" -I"/home/utnso/workspace/directorios-commons" -I"/home/utnso/workspace/commons" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"mapa-commons/map-commons.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


