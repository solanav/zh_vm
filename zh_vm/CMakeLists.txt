# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 2.8)

add_library(bin_utils bin_utils.cpp bin_utils.h)
add_library(Hardware Hardware.cpp Hardware.h)

add_executable (core core.cpp)
target_link_libraries(core bin_utils Hardware)
# TODO: Add tests and install targets if needed.
