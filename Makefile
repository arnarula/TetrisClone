CXX = g++
CXXFLAGS = -leak-check=full -g -std=c++17 -Wall -pedantic
INCFLAGS = -I./inc
LDFLAGS = -lraylib
EXE_DIR = bin
OBJ_DIR = bin/obj
TARGET = $(EXE_DIR)/tetris
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

run: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(EXE_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJ_DIR)
	rm -rf $(EXE_DIR)

rebuild: clean run
