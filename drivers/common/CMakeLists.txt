add_library(drivers_common INTERFACE)

target_sources(
        drivers_common INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/detect_spi_instance.cpp
)

target_include_directories(
        drivers_common INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_common INTERFACE
        pico_stdlib
        hardware_spi
)
