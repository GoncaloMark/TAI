OUTPUT_DIR := bin
INCLUDE_DIR := src/include/


#* INCLUDE OBJS
INCLUDE_SRC := $(shell find $(INCLUDE_DIR) -name '*.cpp')
INCLUDE_C_OBJS := $(INCLUDE_SRC:%.cpp=${OUTPUT_DIR}/%.o)

#* CPM
WAS_CHATTED_OUTPUT_FILE := ${OUTPUT_DIR}/was_chatted
WAS_CHATTED_C_SRC := src/main.cpp src/fcm.cpp
WAS_CHATTED_C_OBJS := $(WAS_CHATTED_C_SRC:%.cpp=${OUTPUT_DIR}/%.o)

#* COMPILER STUFF
CCOMPILER := g++
CFLAGS := -c -O3 -Wall -Werror -I"./src/include" 

#* TARGETS
all: clean was_chatted 

#* TARGETS
was_chatted:	$(WAS_CHATTED_OUTPUT_FILE)

$(INCLUDE_C_OBJS) : $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

$(WAS_CHATTED_C_OBJS): $(OUTPUT_DIR)/%.o : %.cpp
	@echo Compiling: $<
	@mkdir -p $(@D)
	@$(CCOMPILER) $(CFLAGS) -o "$@" "$<"

$(WAS_CHATTED_OUTPUT_FILE) : $(WAS_CHATTED_C_OBJS) $(INCLUDE_C_OBJS)
	@echo Linking: $@
	@$(CCOMPILER) -o "$(WAS_CHATTED_OUTPUT_FILE)" $(WAS_CHATTED_C_OBJS) $(INCLUDE_C_OBJS)

#* CLEAN TARGET
.PHONY: clean
clean: 
	@echo Cleaning bin
	@rm -rf $(OUTPUT_DIR)



