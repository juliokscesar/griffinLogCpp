cmake_minimum_required(VERSION 3.9)

project(griffinLog VERSION 0.1)

set(SRC_FILES src/griffinLog/griffinLog.cpp)

# When using MSVC, it is required to define _CRT_SECURE_NO_WARNINGS to use localtime()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(griffinLog STATIC ${SRC_FILES})
