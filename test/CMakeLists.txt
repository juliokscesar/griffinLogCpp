cmake_minimum_required(VERSION 3.28)
project(grflogTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(test test.cpp)

target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/../src)
target_link_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/../build)
target_link_libraries(test PUBLIC griffinLog)

