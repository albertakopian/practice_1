cmake_minimum_required(VERSION 3.13)
project(untitled25)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled25 main.cpp)

add_subdirectory(googletest-master)
include_directories(googletest-master/googletest/include)
include_directories(googletest-master/googlemock/include)
target_link_libraries(untitled25 gtest gtest_main)
