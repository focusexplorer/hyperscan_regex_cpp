cmake_minimum_required(VERSION 3.15)
project(test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(demo demo.cpp)
find_package(hsc REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../build/install/lib/cmake)
target_link_libraries(demo fe::hsc)
