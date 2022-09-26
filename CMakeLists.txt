cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk)
include(pico_sdk_import.cmake)

set (CMAKE_BUILD_TYPE "Debug")

set(ProjectName "turn_light")
project(${ProjectName})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(src)