################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core.cpp \
../src/crc16.cpp \
../src/main.cpp \
../src/ptracker.cpp \
../src/timer0.cpp \
../src/timer1.cpp \
../src/usart0.cpp 

OBJS += \
./src/core.o \
./src/crc16.o \
./src/main.o \
./src/ptracker.o \
./src/timer0.o \
./src/timer1.o \
./src/usart0.o 

CPP_DEPS += \
./src/core.d \
./src/crc16.d \
./src/main.d \
./src/ptracker.d \
./src/timer0.d \
./src/timer1.d \
./src/usart0.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I/usr/lib/avr/include/avr -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


