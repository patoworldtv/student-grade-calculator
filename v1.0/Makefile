CXX      := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Iinclude

SRC := src/Analyzer.cpp \
       src/Sorter.cpp \
       src/FileGenerator.cpp \
       src/ExceptionHandlers.cpp \
       src/main.cpp \
       src/Person.cpp

OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))
TARGET := analyzer

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
