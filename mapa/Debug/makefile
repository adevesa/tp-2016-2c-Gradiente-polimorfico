################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/pkmn/subdir.mk
-include src/nivel-gui/subdir.mk
-include src/subdir.mk
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
all: mapa

dependents:
	-cd /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons/Debug && $(MAKE) all

# Tool invocations
mapa: $(OBJS) $(USER_OBJS) /home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons/Debug/libnecessary-commons.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons/Debug" -o "mapa" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) mapa
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/necessary-commons/Debug/libnecessary-commons.so:

-include ../makefile.targets
