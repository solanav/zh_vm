# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 2.8)

add_library(types INTERFACE)
add_library(bin_utils bin_utils.cpp bin_utils.h)
add_library(Hardware Hardware.cpp Hardware.h)
add_library(Processor Processor.cpp Processor.h)

target_include_directories(types INTERFACE ./)

add_executable (core core.cpp)
target_link_libraries(core Hardware Processor bin_utils types)

if(MSVC)
	target_compile_options(core PRIVATE /W4)
else(MSVC)
	target_compile_options(core PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)