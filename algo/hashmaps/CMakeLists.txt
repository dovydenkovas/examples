cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDART 23)
set(CMAKE_CXX_STANDART_REQUIRED ON)

project(hashmaps)

add_executable(hashmap src/main.cpp src/hashmap.cpp)
