################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/hardware/LED.c \
../code/hardware/adc.c \
../code/hardware/move_filter.c \
../code/hardware/pwm_input.c 

COMPILED_SRCS += \
./code/hardware/LED.src \
./code/hardware/adc.src \
./code/hardware/move_filter.src \
./code/hardware/pwm_input.src 

C_DEPS += \
./code/hardware/LED.d \
./code/hardware/adc.d \
./code/hardware/move_filter.d \
./code/hardware/pwm_input.d 

OBJS += \
./code/hardware/LED.o \
./code/hardware/adc.o \
./code/hardware/move_filter.o \
./code/hardware/pwm_input.o 


# Each subdirectory must supply rules for building sources it contributes
code/hardware/%.src: ../code/hardware/%.c code/hardware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fE:/Project/23智能车参考资料/无刷电机/TC264_GTM_BLDC_Project-master/【源码】下载到驱动板的源码/seekfree_tc264_bldc_gtm/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/hardware\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/hardware/%.o: ./code/hardware/%.src code/hardware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code-2f-hardware

clean-code-2f-hardware:
	-$(RM) ./code/hardware/LED.d ./code/hardware/LED.o ./code/hardware/LED.src ./code/hardware/adc.d ./code/hardware/adc.o ./code/hardware/adc.src ./code/hardware/move_filter.d ./code/hardware/move_filter.o ./code/hardware/move_filter.src ./code/hardware/pwm_input.d ./code/hardware/pwm_input.o ./code/hardware/pwm_input.src

.PHONY: clean-code-2f-hardware

