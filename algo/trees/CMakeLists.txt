cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project(trees)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(binary_sources src/bin_main.cpp)
set(rb_sources src/rb_main.cpp)

add_executable(binary_tree ${binary_sources})
target_link_libraries(binary_tree Qt::Widgets)

add_executable(rb_tree ${rb_sources})
target_link_libraries(rb_tree Qt::Widgets)
