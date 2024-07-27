CC := gcc
CXX := g++
CFLAGS := -std=c11 -Werror -g -Iinclude -I/usr/local/include/opencv4 -MMD -O3 -march=native
CXXFLAGS := -std=c++17 -Werror -g -Iinclude -I/usr/local/include/opencv4 -MMD -fopenmp -O3 -march=native
LDFLAGS := -L/usr/local/lib/ -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -lfftw3 -fopenmp -lxcb -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
BIN_DIR := bin
SRC_DIR := src
OBJ_DIR := obj
INCLUDE_DIR := include

CPP_SRC := $(shell find $(SRC_DIR) -name '*.cpp')
C_SRC := $(shell find $(SRC_DIR) -name '*.c')

CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRC))
C_OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRC))
OBJ := $(CPP_OBJ) $(C_OBJ)
DEP := $(OBJ:.o=.d)

TARGET := main

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(DEP)

-include $(DEP)

.PHONY: all clean