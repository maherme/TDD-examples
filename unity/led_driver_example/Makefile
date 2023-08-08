# Target path and name definition
TARGET = $(BLD_DIR)/test

# Search for all include path and set in INCLUDE variable
INCLUDE_PATH = $(shell find ./ -type d)
INCLUDE = $(INCLUDE_PATH:%=-I%) 
UNITY_PATH = $(shell find $(UNITY_DIR) -type d)
INCLUDE += $(UNITY_PATH:%=-I%) 

# Needed directories
BLD_DIR = ./build
OBJ_DIR = $(BLD_DIR)/obj
SRC_DIR = ./src
TST_DIR = ./test
UTIL_DIR = ./util
MOCK_DIR = ./mocks
UNITY_DIR = ../unity/src

# Search for source files and define OBJS as list of needed object files
# Source directories are included in VPATH for searching prerequisites
SOURCES = $(shell find $(SRC_DIR) -type f -name "*.c")
SOURCES += $(shell find $(TST_DIR) -type f -name "*.c")
SOURCES += $(shell find $(UNITY_DIR) -type f -name "*.c")
SOURCES += $(shell find $(UTIL_DIR) -type f -name "*.c")
SOURCES += $(shell find $(MOCK_DIR) -type f -name "*.c")
#$(info SOURCES="$(SOURCES)")
VPATH = $(dir $(SOURCES))
#$(info VPATH="$(VPATH)")
OBJECTS = $(notdir $(patsubst %.c, %.o, $(SOURCES)))
OBJS = $(addprefix $(OBJ_DIR)/, $(OBJECTS))
#$(info OBJS="$(OBJS)")

# Set compiler and flags
CC = gcc
CFLAGS = -c -MD $(INCLUDE)

$(TARGET) : $(OBJS)
	@mkdir -p $(BLD_DIR)
	@$(CC) $(OBJS) -o $(TARGET)
	./$(TARGET)

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
