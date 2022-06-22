cmake_minimum_required(VERSION 3.20.2)
project(simulator C)

set(CMAKE_C_STANDARD 11)

add_executable(simulator main.c util.h MEM.h REG.h ALU.h command.h instruction.h)
