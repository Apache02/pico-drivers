set(NAME "example--ssd1306_draw")
add_executable(
        ${NAME}
        ssd1306_draw.cpp
)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

add_compile_definitions(
        DISPLAY_SPI
#        DISPLAY_I2C
        DRAW_CIRCLES
        DRAW_LINES
        DRAW_RECTS
        DRAW_GLYPHS
        DRAW_TEXT
)

pico_add_extra_outputs(${NAME})

target_link_libraries(
        ${NAME}
        pico_stdlib
        drivers_led
        drivers_ssd1306
        drivers_gui
)

add_deploy_target(${NAME} "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2")
