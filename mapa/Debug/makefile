################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include mapa-commons/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libmapa-prueba.so

dependents:
	-cd /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/cliente-servidor/Debug && $(MAKE) all
	-cd /home/utnso/workspace/commons/Debug && $(MAKE) all
	-cd /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/commons_leer_archivos/Debug && $(MAKE) all

# Tool invocations
libmapa-prueba.so: $(OBJS) $(USER_OBJS) /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/cliente-servidor/Debug/libcliente-servidor.so /home/utnso/workspace/commons/Debug/libcommons.so /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/commons_leer_archivos/Debug/libcommons_leer_archivos.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/commons_leer_archivos/Debug" -L"/home/utnso/workspace/commons/Debug" -L"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/cliente-servidor/Debug" -shared -o "libmapa-prueba.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(LIBRARIES) libmapa-prueba.so
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/cliente-servidor/Debug/libcliente-servidor.so:
/home/utnso/workspace/commons/Debug/libcommons.so:
/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/commons_leer_archivos/Debug/libcommons_leer_archivos.so:

-include ../makefile.targets
