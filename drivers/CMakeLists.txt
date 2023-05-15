add_library(drivers INTERFACE)

target_sources(drivers INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/led/led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/led/pwm_led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/led/rgb_led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/knob/ec11.cpp
        )

target_link_libraries(drivers INTERFACE pico_stdlib hardware_spi hardware_pwm)
