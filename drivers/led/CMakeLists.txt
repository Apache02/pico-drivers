add_library(drivers_led INTERFACE)

target_sources(
        drivers_led INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/pwm_led.cpp
        ${CMAKE_CURRENT_LIST_DIR}/rgb_led.cpp
)

target_include_directories(
        drivers_led INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_led INTERFACE
        pico_stdlib
        hardware_pwm
)
