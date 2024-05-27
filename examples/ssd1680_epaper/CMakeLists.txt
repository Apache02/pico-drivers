set(NAME "example--ssd1680_epaper")
add_executable(
        ${NAME}
        ssd1680_epaper.cpp
)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

pico_add_extra_outputs(${NAME})

target_link_libraries(
        ${NAME}
        pico_stdlib
        drivers_led
        drivers_ssd1680
        drivers_gui
)

add_deploy_target(${NAME} "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2")
