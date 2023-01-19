cmake_minimum_required(VERSION 3.9)

project(griffinLog VERSION 0.1)

set(SRC_FILES src/griffinLog/griffinLog.cpp)

add_library(griffinLog STATIC ${SRC_FILES})
