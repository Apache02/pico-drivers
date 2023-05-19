set(NAME "example--ssd1306_test")
add_executable(${NAME}
        ssd1306_test.cpp
        )

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

pico_add_extra_outputs(${NAME})

target_link_libraries(${NAME} pico_stdlib drivers)

add_flash_target(${NAME} "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2")
