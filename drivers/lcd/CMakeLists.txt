add_library(drivers_lcd INTERFACE)

target_sources(
        drivers_lcd INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/io.cpp
        ${CMAKE_CURRENT_LIST_DIR}/display.cpp
)

target_include_directories(
        drivers_lcd INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_lcd INTERFACE
        pico_stdlib
        hardware_gpio
)
