set(NAME "example--st7789_interp")
add_executable(
        ${NAME}
        st7789_interp.cpp
        FpsMeasure.cpp
)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

pico_add_extra_outputs(${NAME})

add_compile_definitions(COPY_IMAGE_TO_RAM)

target_link_libraries(
        ${NAME}
        pico_stdlib
        drivers_led
        drivers_st7789
        hardware_interp
)

add_deploy_target(${NAME} "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2")
