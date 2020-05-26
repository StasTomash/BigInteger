cmake_minimum_required(VERSION 3.14)
project(BigIntegerLab)

set(CMAKE_CXX_STANDARD 17)

add_executable(BigIntegerLab main.cpp BigInteger.h BigInteger.cpp
               Interface.cpp Interface.h BigIntegerAlgorithm.h BigIntegerAlgorithm.cpp)