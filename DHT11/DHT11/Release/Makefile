################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../dht11.c \
../main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
dht11.o \
main.o

OBJS_AS_ARGS +=  \
dht11.o \
main.o

C_DEPS +=  \
dht11.d \
main.d

C_DEPS_AS_ARGS +=  \
dht11.d \
main.d

OUTPUT_FILE_PATH +=DHT11.elf

OUTPUT_FILE_PATH_AS_ARGS +=DHT11.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./dht11.o: .././dht11.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DF_CPU=16000000  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.209\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall  -mmcu=atmega328p  -c -std=gnu99 -save-temps -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DF_CPU=16000000  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.209\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall  -mmcu=atmega328p  -c -std=gnu99 -save-temps -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="DHT11.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections  -mmcu=atmega328p   
	@echo Finished building target: $@
	"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "DHT11.elf" "DHT11.hex"
	"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "DHT11.elf" "DHT11.eep" || exit 0
	"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objdump.exe" -h -S "DHT11.elf" > "DHT11.lss"
	"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "DHT11.elf" "DHT11.srec"
	"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-size.exe" "DHT11.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "DHT11.elf" "DHT11.a" "DHT11.hex" "DHT11.lss" "DHT11.eep" "DHT11.map" "DHT11.srec" "DHT11.usersignatures"
	