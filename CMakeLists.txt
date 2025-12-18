cmake_minimum_required(VERSION 3.27)
project(Wordle_Solver)

set(CMAKE_CXX_STANDARD 14)

file(COPY ${CMAKE_SOURCE_DIR}/data
        DESTINATION ${CMAKE_BINARY_DIR})

include_directories(include)

add_executable(Wordle_Solver
        include/Strategies.h
        include/WordleSolver.h
        src/main.cpp)
