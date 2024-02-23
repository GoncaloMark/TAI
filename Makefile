OUTPUT_DIR := bin

#* INCLUDE OBJS
INCLUDE_SRC := src/include/argparser/argparser.cpp src/include/utils/utils.cpp
INCLUDE_C_OBJS := $(INCLUDE_SRC:%.cpp=${OUTPUT_DIR}/%.o)

#* CPM
CPM_OUTPUT_FILE := ${OUTPUT_DIR}/cpm_model
CPM_C_SRC := src/cpm/main.cpp 
CPM_C_OBJS := $(CPM_C_SRC:%.cpp=${OUTPUT_DIR}/%.o)

#* MUTATE
MUTATE_OUTPUT_FILE := ${OUTPUT_DIR}/mutate_program
MUTATE_C_SRC := src/mutate/main.cpp 
MUTATE_C_OBJS := $(MUTATE_C_SRC:%.cpp=${OUTPUT_DIR}/%.o)

#* COMPILER STUFF
CCOMPILER := g++
CFLAGS := -c -O2 -Wall -Werror -ggdb -c -I"./src/include" 

#* TARGETS
all: clean cpm mutate


#* CPM TARGETS
cpm:	$(CPM_OUTPUT_FILE)

$(INCLUDE_C_OBJS) : $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

$(CPM_C_OBJS): $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

$(CPM_OUTPUT_FILE) : $(CPM_C_OBJS) $(INCLUDE_C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) -o "$(CPM_OUTPUT_FILE)" $(CPM_C_OBJS) $(INCLUDE_C_OBJS)

#* MUTATE TARGETS
mutate:	$(MUTATE_OUTPUT_FILE)

$(MUTATE_C_OBJS): $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

$(MUTATE_OUTPUT_FILE): $(MUTATE_C_OBJS) $(INCLUDE_C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) -o "$(MUTATE_OUTPUT_FILE)" $(MUTATE_C_OBJS) $(INCLUDE_C_OBJS)

#* CLEAN TARGET
.PHONY: clean
clean: 
	@echo Cleaning bin
	@rm -rf $(OUTPUT_DIR)



