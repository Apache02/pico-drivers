add_library(drivers_io INTERFACE)

target_sources(
        drivers_io INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/I2C.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SPI.cpp
)

target_include_directories(
        drivers_io INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_io INTERFACE
        pico_stdlib
        hardware_spi
        hardware_i2c
)
