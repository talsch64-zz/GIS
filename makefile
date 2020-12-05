COMP = g++-10

GOOGLE_LIB = /usr/src/googletest/googletest/gcc-10-build/lib/
GOOGLE_TEST_INCLUDE = /usr/src/googletest/googletest/include/

INC_DIRS := . ./entities ./entities/geometry ./entities/JsonHandlers/Serializers ./search ./bonus.error_handling
SRCS := $(wildcard ./*.cpp ./entities/*.cpp ./entities/geometry/*.cpp ./entities/JsonHandlers/Serializers/*.cpp ./search/*.cpp ./bonus.error_handling/*.cpp) ./tests/tests.cpp
DEPS := $(OBJS:.o=.d)
BUILD_DIR := build
INC_FLAGS := $(addprefix -I,$(INC_DIRS))


#put all your object files here
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
#The executabele filename DON'T CHANGE
EXEC = ex1
CPP_COMP_FLAG = -std=c++20 -Werror -Wall -Wextra -pedantic-errors -DNDEBUG -MMD -MP $(INC_FLAGS) -I$(GOOGLE_TEST_INCLUDE)
CPP_LINK_FLAG = -L $(GOOGLE_LIB) -lgtest -lgtest_main -pthread

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(COMP) $(CPP_COMP_FLAG) -c $< -o $@

#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies


clean:
	rm -f $(OBJS) $(EXEC)

-include $(DEPS)

MKDIR_P ?= mkdir -p