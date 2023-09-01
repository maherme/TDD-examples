# Target path and name definition
TARGET1 = $(BLD_DIR)/good_test
TARGET2 = $(BLD_DIR)/bad_test

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
SOURCES += $(shell find $(TST_DIR) -type f -name "*.cpp")
SOURCES += $(shell find $(MOCK_DIR) -type f -name "*.c")
#$(info SOURCES="$(SOURCES)")
VPATH := $(dir $(SOURCES))
#$(info VPATH="$(VPATH)")
OBJECTS := $(notdir $(filter %.o,$(SOURCES:.cpp=.o) $(SOURCES:.c=.o)))
#$(info OBJECTS="$(OBJECTS)")
OBJS1 := $(addprefix $(OBJ_DIR)/, $(filter-out BadStateMachineTest.o, $(OBJECTS)))
OBJS2 := $(addprefix $(OBJ_DIR)/, $(filter-out GoodStateMachineTest.o, $(OBJECTS)))
#$(info OBJS1="$(OBJS1)")
#$(info OBJS2="$(OBJS2)")

# Set static libraries
LD_LIBRARIES = -L$(CPPUTEST_DIR)/cpputest_build/src/CppUTest -lCppUTest
LD_LIBRARIES += -L$(CPPUTEST_DIR)/cpputest_build/src/CppUTestExt -lCppUTestExt

# Set compiler and flags
CC = g++
CXXFLAGS = -c -g -MD -Wall $(INCLUDE)
CXXFLAGS += -include $(CPPUTEST_DIR)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS = -c -g -MD -Wall $(INCLUDE)

$(TARGET1) : $(OBJS1)
	@mkdir -p $(BLD_DIR)
	@$(CC) $(OBJS1) -o $(TARGET1) $(LD_LIBRARIES)
	./$(TARGET1)

$(TARGET2) : $(OBJS2)
	@mkdir -p $(BLD_DIR)
	@$(CC) $(OBJS2) -o $(TARGET2) $(LD_LIBRARIES)
	./$(TARGET2)

$(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $< -o $@

-include $(OBJ_DIR)/*.d

.PHONY: all
all: $(TARGET1) \
	 $(TARGET2)

.PHONY: good_test
good_test: $(TARGET1)

.PHONY: bad_test
bad_test: $(TARGET2)

.PHONY: clean
clean:
	@rm -rf $(BLD_DIR)

.PHONY: run_good
run_good:
	./$(TARGET1)

.PHONY: run_bad
run_bad:
	./$(TARGET2)

