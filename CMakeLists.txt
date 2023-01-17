cmake_minimum_required(VERSION 3.9)

project(griffinLog VERSION 0.1)

set(SRC_FILES src/griffinLog/griffinLog.cpp)

# When using MSVC, it is required to define _CRT_NO_VA_START_VALIDATION to use va_start() with const references
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_NO_VA_START_VALIDATION)
endif()

add_library(griffinLog STATIC ${SRC_FILES})
