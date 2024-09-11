cmake_minimum_required(VERSION 3.12)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)
    include(config.cmake)
endif()

include(pico_sdk_import.cmake)
include(scripts/functions.cmake)

project(pico-drivers C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

set(PICO_LIB_DRIVERS_PATH ${CMAKE_SOURCE_DIR})
include(pico_lib_drivers_import.cmake)

add_subdirectory(examples)
