################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ov2640/ov2640.c 

OBJS += \
./src/ov2640/ov2640.o 

C_DEPS += \
./src/ov2640/ov2640.d 


# Each subdirectory must supply rules for building sources it contributes
src/ov2640/%.o: ../src/ov2640/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -IE:/pynqz2cam/design_1_wrapper/export/design_1_wrapper/sw/design_1_wrapper/standalone_domain/bspinclude/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


