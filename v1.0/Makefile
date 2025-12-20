CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Iinclude

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))
TARGET := student-grade-calculator

.PHONY: all clean run

all: $(TARGET)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

clean:
	rm -rf build $(TARGET)

run: all
	./$(TARGET)
