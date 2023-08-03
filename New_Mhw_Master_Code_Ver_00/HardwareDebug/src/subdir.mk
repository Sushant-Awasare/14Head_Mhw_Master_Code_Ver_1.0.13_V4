################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/New_Mhw_Master_Code_Ver_00.c \
../src/calibration_code.c \
../src/can_code.c \
../src/data_flash_code.c \
../src/delay_code.c \
../src/manual_code.c \
../src/modbus_calculations.c \
../src/modbus_code.c \
../src/modbus_records.c \
../src/modbus_responses.c \
../src/runcycle_code.c 

COMPILER_OBJS += \
src/New_Mhw_Master_Code_Ver_00.obj \
src/calibration_code.obj \
src/can_code.obj \
src/data_flash_code.obj \
src/delay_code.obj \
src/manual_code.obj \
src/modbus_calculations.obj \
src/modbus_code.obj \
src/modbus_records.obj \
src/modbus_responses.obj \
src/runcycle_code.obj 

C_DEPS += \
src/New_Mhw_Master_Code_Ver_00.d \
src/calibration_code.d \
src/can_code.d \
src/data_flash_code.d \
src/delay_code.d \
src/manual_code.d \
src/modbus_calculations.d \
src/modbus_code.d \
src/modbus_records.d \
src/modbus_responses.d \
src/runcycle_code.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrx -subcommand="src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

