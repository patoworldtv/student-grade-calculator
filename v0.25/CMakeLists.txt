cmake_minimum_required(VERSION 3.15)

# Project name and language
project(StudentProcessor_v0_25 LANGUAGES CXX)

# Require modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Person.cpp
    src/StudentManager.cpp
    src/Timer.cpp
    src/ContainerManager.cpp
)

set(HEADERS
    src/Person.h
    src/StudentManager.h
    src/Timer.h
    src/ContainerManager.h
)

# Create executable
add_executable(student_v0_25 ${SOURCES} ${HEADERS})

# Enable warnings
if (MSVC)
    target_compile_options(student_v0_25 PRIVATE /W4 /permissive-)
else()
    target_compile_options(student_v0_25 PRIVATE -Wall -Wextra -pedantic)
endif()
