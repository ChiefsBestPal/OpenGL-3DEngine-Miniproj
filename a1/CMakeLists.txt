cmake_minimum_required(VERSION 3.29)
project(a1)

set(CMAKE_CXX_STANDARD 20)

add_executable(a1 main.cpp Part1.cpp
        Part1.h
        Part2.cpp
        Part2.h
)
