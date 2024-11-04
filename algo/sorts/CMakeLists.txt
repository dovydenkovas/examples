cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDART 23)
set(CMAKE_CXX_STANDART_REQUIRED ON)

project(hashmaps)

add_executable(count_sort src/count_sort.cpp)
add_executable(dipers_sort src/dipers_sort.cpp)
add_executable(heap_sort src/heap_sort.cpp)
add_executable(insertion_sort src/insertion_sort.cpp)
add_executable(merge_sort src/merge_sort.cpp)
add_executable(quick_sort src/quick_sort.cpp)
add_executable(selection_sort src/selection_sort.cpp)
add_executable(shell_sort_sort src/shell_sort.cpp)
add_executable(timsort src/timsort.cpp)
