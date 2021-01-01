COMP = g++-10

SOURCE_FILES := $(notdir $(shell find . -name '*.cpp'))
OBJS_FILES := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR := bin
OBJS := $(addprefix $(BIN_DIR)/, $(OBJS_FILES))
SHARED_OBJ := GIS_098765432.so
OUTPUT_DIR := ../../OUTPUT/GIS
OUTPUT_SHARED_OBJ := $(OUTPUT_DIR)/$(SHARED_OBJ)

CPP_COMP_FLAG := -std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
CPP_LINK_FLAG := -shared 

all: $(BIN_DIR) $(OUTPUT_DIR) $(OUTPUT_SHARED_OBJ)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_SHARED_OBJ): $(OBJS)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^ #$(addprefix $(BIN_DIR)/, $^)

# Make sure to have all your dependencies for each .o file creation rule

$(BIN_DIR)/GIS.o: GIS.cpp GIS.h ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

$(BIN_DIR)/Way.o: Way.cpp Way.h ../Common/*.h
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_SHARED_OBJ)
