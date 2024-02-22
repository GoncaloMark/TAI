OUTPUT_DIR := bin
CPM_OUTPUT_FILE := ${OUTPUT_DIR}/cpm_model

CPM_C_SRC := cpm/src/main.cpp cpm/src/include/argparser/argparser.cpp

CPM_C_OBJS := $(CPM_C_SRC:%.cpp=${OUTPUT_DIR}/%.o)

MUTATE_OUTPUT_FILE := ${OUTPUT_DIR}/mutate_program

MUTATE_C_SRC := mutate/src/main.cpp mutate/src/include/argparser/argparser.cpp

MUTATE_C_OBJS := $(MUTATE_C_SRC:%.cpp=${OUTPUT_DIR}/%.o)

CCOMPILER := g++
CFLAGS := -c -O2 -Wall -Werror -ggdb -c -I"./src/include" 


cpm:	clean $(CPM_OUTPUT_FILE)

$(CPM_C_OBJS): $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

$(CPM_OUTPUT_FILE) : $(CPM_C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) -o "$(CPM_OUTPUT_FILE)" $(CPM_C_OBJS) 

mutate:	clean $(MUTATE_OUTPUT_FILE)

$(MUTATE_C_OBJS): $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

$(MUTATE_OUTPUT_FILE) : $(MUTATE_C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) -o "$(MUTATE_OUTPUT_FILE)" $(MUTATE_C_OBJS) 


.PHONY: clean
clean: 
	@echo Cleaning bin
	@rm -rf $(OUTPUT_DIR)



