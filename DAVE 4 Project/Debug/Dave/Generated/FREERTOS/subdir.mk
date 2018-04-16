################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Dave/Generated/FREERTOS/portasm.s 

C_SRCS += \
../Dave/Generated/FREERTOS/croutine.c \
../Dave/Generated/FREERTOS/event_groups.c \
../Dave/Generated/FREERTOS/freertos.c \
../Dave/Generated/FREERTOS/heap.c \
../Dave/Generated/FREERTOS/list.c \
../Dave/Generated/FREERTOS/mpu_wrappers.c \
../Dave/Generated/FREERTOS/port.c \
../Dave/Generated/FREERTOS/queue.c \
../Dave/Generated/FREERTOS/tasks.c \
../Dave/Generated/FREERTOS/timers.c 

OBJS += \
./Dave/Generated/FREERTOS/croutine.o \
./Dave/Generated/FREERTOS/event_groups.o \
./Dave/Generated/FREERTOS/freertos.o \
./Dave/Generated/FREERTOS/heap.o \
./Dave/Generated/FREERTOS/list.o \
./Dave/Generated/FREERTOS/mpu_wrappers.o \
./Dave/Generated/FREERTOS/port.o \
./Dave/Generated/FREERTOS/portasm.o \
./Dave/Generated/FREERTOS/queue.o \
./Dave/Generated/FREERTOS/tasks.o \
./Dave/Generated/FREERTOS/timers.o 

S_DEPS += \
./Dave/Generated/FREERTOS/portasm.d 

C_DEPS += \
./Dave/Generated/FREERTOS/croutine.d \
./Dave/Generated/FREERTOS/event_groups.d \
./Dave/Generated/FREERTOS/freertos.d \
./Dave/Generated/FREERTOS/heap.d \
./Dave/Generated/FREERTOS/list.d \
./Dave/Generated/FREERTOS/mpu_wrappers.d \
./Dave/Generated/FREERTOS/port.d \
./Dave/Generated/FREERTOS/queue.d \
./Dave/Generated/FREERTOS/tasks.d \
./Dave/Generated/FREERTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/FREERTOS/%.o: ../Dave/Generated/FREERTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Dave/Generated/GUI_SEGGERLIBRARY/Config" -I"C:/Dave 4/eclipse//emWin/Start/GUI/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O3 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
Dave/Generated/FREERTOS/%.o: ../Dave/Generated/FREERTOS/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC Assembler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -x assembler-with-cpp -DXMC4700_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -Wall -Wa,-adhlns="$@.lst" -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

