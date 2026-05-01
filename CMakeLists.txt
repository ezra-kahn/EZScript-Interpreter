cmake_minimum_required(VERSION 3.30)
project(interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(interpreter main.cpp
        Token.cpp
        Token.h
)
