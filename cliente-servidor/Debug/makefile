################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
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
all: libcliente-servidor.so

dependents:
	-cd /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/mapa/Debug && $(MAKE) all

# Tool invocations
libcliente-servidor.so: $(OBJS) $(USER_OBJS) /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/mapa/Debug/libmapa-prueba.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -shared -o "libcliente-servidor.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(LIBRARIES) libcliente-servidor.so
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/mapa/Debug/libmapa-prueba.so:

-include ../makefile.targets
