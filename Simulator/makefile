COMP = g++-10

SOURCE_FILES := $(notdir $(shell find . -name '*.cpp'))
OBJS := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR := bin
EXEC := simulator_098765432
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


# Make sure to have all your dependencies for each .o file creation rule

$(BIN_DIR)/main.o: main.cpp ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/GISRegistration.o: GISRegistration.cpp ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/NavigationRegistration.o: NavigationRegistration.cpp ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/NavigationValidator.o: NavigationValidator.cpp ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_EXEC)
