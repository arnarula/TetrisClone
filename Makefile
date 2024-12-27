CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -pedantic
INCFLAGS = -I./inc
LDFLAGS = -lraylib

EXE_DIR = bin
OBJ_DIR = bin/obj
SRC_DIR = src

TARGET = $(EXE_DIR)/tetris
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

run: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(EXE_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJ_DIR)
	rm -rf $(EXE_DIR)

rebuild: clean run
