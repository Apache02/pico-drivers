add_library(drivers_ssd1306 INTERFACE)

target_sources(
        drivers_ssd1306 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/io.cpp
        ${CMAKE_CURRENT_LIST_DIR}/display.cpp
)

target_include_directories(
        drivers_ssd1306 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_ssd1306 INTERFACE
        pico_stdlib
        drivers_io
)
