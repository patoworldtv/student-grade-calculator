cmake_minimum_required(VERSION 3.15)
project(student_grade_calculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SRC_FILES "src/*.cpp")

add_executable(student-grade-calculator ${SRC_FILES})
