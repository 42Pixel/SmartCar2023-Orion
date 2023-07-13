################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/IMU.c \
../code/Key.c \
../code/Motor.c \
../code/Run.c \
../code/UI.c \
../code/VOFA.c 

COMPILED_SRCS += \
./code/IMU.src \
./code/Key.src \
./code/Motor.src \
./code/Run.src \
./code/UI.src \
./code/VOFA.src 

C_DEPS += \
./code/IMU.d \
./code/Key.d \
./code/Motor.d \
./code/Run.d \
./code/UI.d \
./code/VOFA.d 

OBJS += \
./code/IMU.o \
./code/Key.o \
./code/Motor.o \
./code/Run.o \
./code/UI.o \
./code/VOFA.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fE:/Project/ADS/WorkSpace/Seekfree_TC264_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/IMU.d ./code/IMU.o ./code/IMU.src ./code/Key.d ./code/Key.o ./code/Key.src ./code/Motor.d ./code/Motor.o ./code/Motor.src ./code/Run.d ./code/Run.o ./code/Run.src ./code/UI.d ./code/UI.o ./code/UI.src ./code/VOFA.d ./code/VOFA.o ./code/VOFA.src

.PHONY: clean-code

