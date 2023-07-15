################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/BLDC-HardWare/adc.c \
../code/BLDC-HardWare/gpio.c \
../code/BLDC-HardWare/move_filter.c \
../code/BLDC-HardWare/pwm_input.c 

COMPILED_SRCS += \
./code/BLDC-HardWare/adc.src \
./code/BLDC-HardWare/gpio.src \
./code/BLDC-HardWare/move_filter.src \
./code/BLDC-HardWare/pwm_input.src 

C_DEPS += \
./code/BLDC-HardWare/adc.d \
./code/BLDC-HardWare/gpio.d \
./code/BLDC-HardWare/move_filter.d \
./code/BLDC-HardWare/pwm_input.d 

OBJS += \
./code/BLDC-HardWare/adc.o \
./code/BLDC-HardWare/gpio.o \
./code/BLDC-HardWare/move_filter.o \
./code/BLDC-HardWare/pwm_input.o 


# Each subdirectory must supply rules for building sources it contributes
code/BLDC-HardWare/%.src: ../code/BLDC-HardWare/%.c code/BLDC-HardWare/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fE:/Project/ADS/WorkSpace/Seekfree_TC264_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/BLDC-HardWare\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/BLDC-HardWare/%.o: ./code/BLDC-HardWare/%.src code/BLDC-HardWare/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code-2f-BLDC-2d-HardWare

clean-code-2f-BLDC-2d-HardWare:
	-$(RM) ./code/BLDC-HardWare/adc.d ./code/BLDC-HardWare/adc.o ./code/BLDC-HardWare/adc.src ./code/BLDC-HardWare/gpio.d ./code/BLDC-HardWare/gpio.o ./code/BLDC-HardWare/gpio.src ./code/BLDC-HardWare/move_filter.d ./code/BLDC-HardWare/move_filter.o ./code/BLDC-HardWare/move_filter.src ./code/BLDC-HardWare/pwm_input.d ./code/BLDC-HardWare/pwm_input.o ./code/BLDC-HardWare/pwm_input.src

.PHONY: clean-code-2f-BLDC-2d-HardWare

