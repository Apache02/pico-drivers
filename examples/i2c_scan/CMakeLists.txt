set(NAME "example--i2c_scan")
add_executable(
        ${NAME}
        i2c_scan.cpp
)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

pico_add_extra_outputs(${NAME})

target_link_libraries(
        ${NAME}
        pico_stdlib
        drivers_led
        drivers_io
)

add_deploy_target(${NAME} "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2")
