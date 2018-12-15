cmake_minimum_required(VERSION 3.13)
project(wet2)

set(CMAKE_CXX_STANDARD 14)

add_executable(wet2 main.cpp ATM.h Bank.h Account.h Account.cpp Bank.cpp ATM.cpp)