# Target path and name definition
TARGET = $(BLD_DIR)/test

# Search for all include path and set in INCLUDE variable
INCLUDE_PATH := $(shell find ./ -type d)
INCLUDE = $(INCLUDE_PATH:%=-I%)
INCLUDE += -I$(CPPUTEST_DIR)/include

# Needed directories
BLD_DIR = ./build
OBJ_DIR = $(BLD_DIR)/obj
SRC_DIR = ./src
TST_DIR = ./test
MOCK_DIR = ./mocks
CPPUTEST_DIR = ../cpputest

# Search for source files and define OBJS as list of needed object files
# Source directories are included in VPATH for searching prerequisites
SOURCES = $(shell find $(SRC_DIR) -type f -name "*.c")
SOURCES += $(shell find $(MOCK_DIR) -type f -name "*.c")
SOURCES += $(shell find $(TST_DIR) -type f -name "*.cpp")
#$(info SOURCES="$(SOURCES)")
VPATH := $(dir $(SOURCES))
#$(info VPATH="$(VPATH)")
OBJECTS := $(notdir $(filter %.o,$(SOURCES:.cpp=.o) $(SOURCES:.c=.o)))
#$(info OBJECTS="$(OBJECTS)")
OBJS := $(addprefix $(OBJ_DIR)/, $(filter-out IO.o, $(OBJECTS)))
#$(info OBJS="$(OBJS)")

# Set static libraries
LD_LIBRARIES = -L$(CPPUTEST_DIR)/cpputest_build/src/CppUTest -lCppUTest
LD_LIBRARIES += -L$(CPPUTEST_DIR)/cpputest_build/src/CppUTestExt -lCppUTestExt

# Set compiler and flags
CC = g++
CXXFLAGS = -c -g -MD -Wall $(INCLUDE)
CXXFLAGS += -include $(CPPUTEST_DIR)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS = -c -g -MD -Wall $(INCLUDE)

$(TARGET) : $(OBJS)
	@mkdir -p $(BLD_DIR)
	@$(CC) $(OBJS) -o $(TARGET) $(LD_LIBRARIES)
	./$(TARGET)

$(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $< -o $@

-include $(OBJ_DIR)/*.d

all: $(TARGET)

.PHONY: clean
clean:
	@rm -rf $(BLD_DIR)

.PHONY: run
run:
	./$(TARGET)
