cmake_minimum_required(VERSION 3.10)
project(StudentAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -Wall")

include_directories(include)

file(GLOB SRC "src/*.cpp")

add_executable(StudentAnalyzer ${SRC})
