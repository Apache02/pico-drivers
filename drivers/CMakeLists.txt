add_library(drivers INTERFACE)

target_sources(drivers INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/led/led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/led/pwm_led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/led/rgb_led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/knob/ec11.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ws2812/PioData.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ws2812/Single.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ws2812/Circle12.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ssd1306/device_io.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ssd1306/Ssd1306.cpp
        ${CMAKE_CURRENT_LIST_DIR}/gui/Gui.cpp
        )

target_link_libraries(drivers INTERFACE pico_stdlib hardware_i2c hardware_spi hardware_pwm hardware_pio)
