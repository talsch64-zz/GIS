COMP = g++-10

SOURCE_FILES := $(wildcard *.cpp ../UserCommon/Utils.cpp)
OBJS := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR := bin
EXEC := simulator_315524694
OUTPUT_DIR := ../../OUTPUT
OUTPUT_EXEC := $(OUTPUT_DIR)/$(EXEC) 

CPP_COMP_FLAG = -std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG 
CPP_LINK_FLAG = -rdynamic -pthread -ldl

all: $(BIN_DIR) $(OUTPUT_DIR) $(OUTPUT_EXEC)

$(BIN_DIR): 
	mkdir -p $(BIN_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_EXEC): $(addprefix $(BIN_DIR)/, $(OBJS))
	$(COMP) $^ $(CPP_LINK_FLAG) -o $@

$(BIN_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@


clean:
	rm -rf $(BIN_DIR) $(OUTPUT_EXEC)
