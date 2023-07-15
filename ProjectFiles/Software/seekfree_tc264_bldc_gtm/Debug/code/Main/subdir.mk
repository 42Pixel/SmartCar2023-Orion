################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Main/IMU.c \
../code/Main/Key.c \
../code/Main/MotorCtrl.c \
../code/Main/Run.c \
../code/Main/UI.c \
../code/Main/VOFA.c 

COMPILED_SRCS += \
./code/Main/IMU.src \
./code/Main/Key.src \
./code/Main/MotorCtrl.src \
./code/Main/Run.src \
./code/Main/UI.src \
./code/Main/VOFA.src 

C_DEPS += \
./code/Main/IMU.d \
./code/Main/Key.d \
./code/Main/MotorCtrl.d \
./code/Main/Run.d \
./code/Main/UI.d \
./code/Main/VOFA.d 

OBJS += \
./code/Main/IMU.o \
./code/Main/Key.o \
./code/Main/MotorCtrl.o \
./code/Main/Run.o \
./code/Main/UI.o \
./code/Main/VOFA.o 


# Each subdirectory must supply rules for building sources it contributes
code/Main/%.src: ../code/Main/%.c code/Main/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fE:/Project/23智能车参考资料/无刷电机/TC264_GTM_BLDC_Project-master/【源码】下载到驱动板的源码/seekfree_tc264_bldc_gtm/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/Main\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/Main/%.o: ./code/Main/%.src code/Main/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code-2f-Main

clean-code-2f-Main:
	-$(RM) ./code/Main/IMU.d ./code/Main/IMU.o ./code/Main/IMU.src ./code/Main/Key.d ./code/Main/Key.o ./code/Main/Key.src ./code/Main/MotorCtrl.d ./code/Main/MotorCtrl.o ./code/Main/MotorCtrl.src ./code/Main/Run.d ./code/Main/Run.o ./code/Main/Run.src ./code/Main/UI.d ./code/Main/UI.o ./code/Main/UI.src ./code/Main/VOFA.d ./code/Main/VOFA.o ./code/Main/VOFA.src

.PHONY: clean-code-2f-Main

