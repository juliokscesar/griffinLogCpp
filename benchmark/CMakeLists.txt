cmake_minimum_required(VERSION 3.28)
project(grflog_benchmark)

set(CMAKE_CXX_STANDARD 20)

add_executable(benchmark benchmark.cpp)

target_include_directories(benchmark PUBLIC ${CMAKE_SOURCE_DIR}/../src)
target_link_directories(benchmark PUBLIC ${CMAKE_SOURCE_DIR}/../build)
target_link_libraries(benchmark PUBLIC griffinLog)

