add_library(drivers_st7789 INTERFACE)

target_sources(
        drivers_st7789 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/io.cpp
        ${CMAKE_CURRENT_LIST_DIR}/display.cpp
)

target_include_directories(
        drivers_st7789 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_st7789 INTERFACE
        pico_stdlib
        hardware_spi
        hardware_dma
        drivers_common
        drivers_io
)
