COMP = g++-10

SOURCE_FILES := $(wildcard GIS.cpp ../UserCommon/entities/*.cpp ../UserCommon/entities/geometry/*.cpp ../UserCommon/entities/JsonHandlers/Serializers/*.cpp ../UserCommon/entities/JsonHandlers/parsers/*.cpp search/*.cpp ../UserCommon/*.cpp)
OBJS_FILES := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR := bin
OBJS := $(addprefix $(BIN_DIR)/, $(OBJS_FILES))
SHARED_OBJ := GIS.so
OUTPUT_DIR := ../../OUTPUT/GIS
# OUTPUT_DIR := .
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

# c++ source
$(BIN_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_SHARED_OBJ)
