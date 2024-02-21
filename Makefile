OUTPUT_DIR := Debug
OUTPUT_FILE := ${OUTPUT_DIR}/tai

C_SRC := src/main.cpp

C_OBJS := $(C_SRC:%.cpp=${OUTPUT_DIR}/%.o)

CCOMPILER := g++
CFLAGS := -c -O2 -Wall -Werror -ggdb -c -I"./src/include" 

# compile

$(C_OBJS): $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

# link

all:	clean $(OUTPUT_FILE)

$(OUTPUT_FILE) : $(C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) -o "$(OUTPUT_FILE)" $(C_OBJS) 

.PHONY: clean
clean: 
	@echo Cleaning Debug
	@rm -rf $(OUTPUT_DIR)



