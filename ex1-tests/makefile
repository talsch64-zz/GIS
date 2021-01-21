COMP = g++-10

GOOGLE_LIB = /usr/src/googletest/googletest/gcc-10-build/lib/
GOOGLE_TEST_INCLUDE = /usr/src/googletest/googletest/include/

PROJECT_DIR := ../GIS
SOURCE_FILES := $(wildcard ../GIS/GIS_315524694.cpp ../UserCommon/entities/*.cpp ../UserCommon/entities/geometry/*.cpp ../UserCommon/entities/JsonHandlers/Serializers/*.cpp ../UserCommon/entities/JsonHandlers/parsers/*.cpp ../GIS/search/*.cpp ../UserCommon/*.cpp) tests.cpp main.cpp
OBJS_FILES := $(SOURCE_FILES:%.cpp=%.o)
BIN_DIR := bin
OBJS := $(addprefix $(BIN_DIR)/, $(OBJS_FILES))
EXEC := ex1
CPP_COMP_FLAG := -std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG -I$(GOOGLE_TEST_INCLUDE) 
CPP_LINK_FLAG := -L$(GOOGLE_LIB) -lgtest -lgtest_main -pthread

all: $(BIN_DIR) $(EXEC)

$(BIN_DIR):
	mkdir -p $(BIN_DIR) && echo $(OBJS_FILES)
 
 $(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@

$(BIN_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(COMP) $(CPP_COMP_FLAG) -D EX1_EX2 -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(EXEC)
