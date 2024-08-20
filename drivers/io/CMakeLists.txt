add_library(drivers_io INTERFACE)

pico_generate_pio_header(drivers_io ${CMAKE_CURRENT_LIST_DIR}/i2c.pio)

target_sources(
        drivers_io INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/I2C.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SPI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/private/pio_i2c.c
        ${CMAKE_CURRENT_LIST_DIR}/PioI2C.cpp
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
        hardware_pio
)
