set(NAME "example--ssd1306_bouncing_ball")
add_executable(
        ${NAME}
        ssd1306_bouncing_ball.cpp
)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

add_compile_definitions(
#        DISPLAY_SPI
        DISPLAY_I2C
#        DISPLAY_PIO_I2C
)

pico_add_extra_outputs(${NAME})

target_link_libraries(
        ${NAME}
        pico_stdlib
        pico_rand
        drivers_led
        drivers_ssd1306
        drivers_gui
)

add_deploy_target(${NAME} "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2")
