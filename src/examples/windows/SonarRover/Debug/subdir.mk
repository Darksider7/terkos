################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sonarrover.cpp 

OBJS += \
./sonarrover.o 

CPP_DEPS += \
./sonarrover.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: TerkOS C++ Compiler (Cygwin)'
	/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/bin/g++ -I/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/include -I/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/usr/include -I/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/include/terk -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


