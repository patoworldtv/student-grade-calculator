CXX = g++
CXXFLAGS = -std=c++17 -I src

SRC = src/main.cpp src/Person.cpp
OUT = build/student-grade-calculator

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
