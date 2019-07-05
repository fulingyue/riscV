cmake_minimum_required(VERSION 3.5)
project(Risc_V)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp Simulator.cpp Simulator.h)
target_compile_options(code PRIVATE -O2)
