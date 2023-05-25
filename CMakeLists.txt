cmake_minimum_required(VERSION 3.16)
project(Toy_12306)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)

add_executable(code
        src/BPT.h src/UserSystem.h src/Calendar.h src/utils.h src/vector.h src/exceptions.h src/map.h src/main.cpp src/TrainSystem.h)
