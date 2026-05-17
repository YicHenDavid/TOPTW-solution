cmake_minimum_required(VERSION 3.14)
project(toptw VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(toptw main.cpp)
target_include_directories(toptw PRIVATE ${CMAKE_SOURCE_DIR})
