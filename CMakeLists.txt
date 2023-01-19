cmake_minimum_required(VERSION 3.9)

project(griffinLog VERSION 0.1)

# CMAKE_BUILD_TYPE set to Debug by default. If you want release, just use "cmake .. -DCMAKE_BUILD_TYPE=Release"
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_definitions(-DGRIFFIN_LOG_DEBUG)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SRC_FILES src/griffinLog/griffinLog.cpp)

add_library(griffinLog STATIC ${SRC_FILES})
